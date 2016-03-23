/*********************************************************************/
/* Copyright (c) 2014, EPFL/Blue Brain Project                       */
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

#ifndef MARKERS_H
#define MARKERS_H

#include "types.h"

#include "serializationHelpers.h"

#include <QObject>
#include <QPointF>
#include <QAbstractListModel>
#include <map>

#include <boost/serialization/access.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/serialization/map.hpp>

/**
 * Store Markers to display user interaction.
 */
class Markers : public QAbstractListModel,
                public boost::enable_shared_from_this<Markers>
{
    Q_OBJECT

public:

    enum MarkerRoles
    {
        XPOSITION_ROLE = Qt::UserRole,
        YPOSITION_ROLE
    };

    /** Constructor */
    Markers( QObject* parentObject = 0 );

    QVariant data( const QModelIndex& index, int role ) const override;
    int rowCount( const QModelIndex& parent = QModelIndex( )) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void addMarker( int id, const QPointF& position );
    void updateMarker( int id, const QPointF& position );
    void removeMarker( int id );

signals:
    void updated( MarkersPtr markers );

private:
    typedef std::pair<int, QPointF> Marker;
    typedef std::vector<Marker> MarkersVector;

    MarkersVector::iterator _findMarker( const int id );
    Q_DISABLE_COPY( Markers )

    friend class boost::serialization::access;

    template<class Archive>
    void serialize( Archive & ar, const unsigned int )
    {
        ar & _markers;
    }

    MarkersVector _markers;
};

#endif // MARKERS_H
