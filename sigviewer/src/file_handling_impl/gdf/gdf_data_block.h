/*
    Copyright (C) by several contributors before 2012
    Copyright (C) Alois Schloegl 2012
    This is part of a patch against "SigViewer -r 557", 
    maintained at http://biosig.sf.net/ 

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 3
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>. 
*/

#ifndef GDF_DATA_BLOCK_H
#define GDF_DATA_BLOCK_H

#include "base/data_block.h"
#include "gdf_signal_cache.h"

#include "GDF/Reader.h"

#include <QVector>
#include <QSharedPointer>

namespace SigViewer_
{

//-------------------------------------------------------------------------------------------------
class GDFDataBlock : public DataBlock
{
public:
    //---------------------------------------------------------------------------------------------
    GDFDataBlock (QSharedPointer<GDFSignalCache> cache, ChannelID channel, size_t length, float64 sample_rate);

    //---------------------------------------------------------------------------------------------
    virtual ~GDFDataBlock ();

    //---------------------------------------------------------------------------------------------
    virtual QSharedPointer<DataBlock> createSubBlock (size_t start, size_t  length) const;

    //---------------------------------------------------------------------------------------------
    virtual float32 const& operator[] (size_t index) const;

    //-------------------------------------------------------------------------
    virtual float32 getMin () const;

    //-------------------------------------------------------------------------
    virtual float32 getMax () const;

    //-------------------------------------------------------------------------
    void addDownSampledVersion (QSharedPointer<DataBlock> data, unsigned downsampling_factor);

    //-------------------------------------------------------------------------
    std::pair<QSharedPointer<DataBlock>, unsigned> getNearbyDownsampledBlock (unsigned downsampling_factor) const;

private:
    //---------------------------------------------------------------------------------------------
    GDFDataBlock (GDFDataBlock const& src, size_t start_sample, size_t length);

    //---------------------------------------------------------------------------------------------
    Q_DISABLE_COPY (GDFDataBlock);

    //---------------------------------------------------------------------------------------------
    ChannelID channel_;
    unsigned start_sample_;
    mutable float64 current_min_;
    mutable float64 current_max_;
    mutable float64 current_value_;

    QMap<unsigned, QSharedPointer<DataBlock> > downsampled_map_;

    mutable QSharedPointer<GDFSignalCache> cache_;
};

} // namespace SigViewer_

#endif // GDF_DATA_BLOCK_H
