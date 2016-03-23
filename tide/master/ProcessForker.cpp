/*********************************************************************/
/* Copyright (c) 2016, EPFL/Blue Brain Project                       */
/*                     Raphael Dumusc <raphael.dumusc@epfl.ch>       */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/*   1. Redistributions of source code must retain the above         */
/*      copyright notice, this list of conditions and the following  */
/*      disclaimer.                                                  */
/*                                                                   */
/*   2. Redistributions in binary form must reproduce the above      */
/*      copyright notice, this list of conditions and the following  */
/*      disclaimer in the documentation and/or other materials       */
/*      provided with the distribution.                              */
/*                                                                   */
/*    THIS  SOFTWARE IS PROVIDED  BY THE  UNIVERSITY OF  TEXAS AT    */
/*    AUSTIN  ``AS IS''  AND ANY  EXPRESS OR  IMPLIED WARRANTIES,    */
/*    INCLUDING, BUT  NOT LIMITED  TO, THE IMPLIED  WARRANTIES OF    */
/*    MERCHANTABILITY  AND FITNESS FOR  A PARTICULAR  PURPOSE ARE    */
/*    DISCLAIMED.  IN  NO EVENT SHALL THE UNIVERSITY  OF TEXAS AT    */
/*    AUSTIN OR CONTRIBUTORS BE  LIABLE FOR ANY DIRECT, INDIRECT,    */
/*    INCIDENTAL,  SPECIAL, EXEMPLARY,  OR  CONSEQUENTIAL DAMAGES    */
/*    (INCLUDING, BUT  NOT LIMITED TO,  PROCUREMENT OF SUBSTITUTE    */
/*    GOODS  OR  SERVICES; LOSS  OF  USE,  DATA,  OR PROFITS;  OR    */
/*    BUSINESS INTERRUPTION) HOWEVER CAUSED  AND ON ANY THEORY OF    */
/*    LIABILITY, WHETHER  IN CONTRACT, STRICT  LIABILITY, OR TORT    */
/*    (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY WAY OUT    */
/*    OF  THE  USE OF  THIS  SOFTWARE,  EVEN  IF ADVISED  OF  THE    */
/*    POSSIBILITY OF SUCH DAMAGE.                                    */
/*                                                                   */
/* The views and conclusions contained in the software and           */
/* documentation are those of the authors and should not be          */
/* interpreted as representing official policies, either expressed   */
/* or implied, of Ecole polytechnique federale de Lausanne.          */
/*********************************************************************/

#include "ProcessForker.h"

#include "MPIChannel.h"
#include "log.h"
#include "serializationHelpers.h"

#include <QProcess>

ProcessForker::ProcessForker( MPIChannelPtr mpiChannel )
    : _mpiChannel( mpiChannel )
    , _processMessages( true )
{}

void ProcessForker::run()
{
    while( _processMessages )
    {
        const ProbeResult result = _mpiChannel->probe();
        if( !result.isValid( ))
        {
            put_flog( LOG_ERROR, "Invalid probe result size: %d", result.size );
            continue;
        }

        _buffer.setSize( result.size );
        _mpiChannel->receive( _buffer.data(), result.size, result.src,
                              result.message );

        switch( result.message )
        {
        case MPI_MESSAGE_TYPE_START_PROCESS:
        {
            QString string;
            _buffer.deserialize( string );
            QStringList args = string.split( '#' );
            if( args.length() != 2 )
            {
                put_flog( LOG_WARN, "Invalid command: '%d'",
                          string.toLocal8Bit().constData( ));
                break;
            }
            _launch( args[0], args[1] );
            break;
        }
        case MPI_MESSAGE_TYPE_QUIT:
            _processMessages = false;
            break;
        default:
            put_flog( LOG_WARN, "Invalid message type: '%d'", result.message );
            break;
        }
    }
}

void ProcessForker::_launch( const QString& command, const QString& workingDir )
{
    QProcess* process = new QProcess();
    process->setWorkingDirectory( workingDir );
    process->startDetached( command );
}
