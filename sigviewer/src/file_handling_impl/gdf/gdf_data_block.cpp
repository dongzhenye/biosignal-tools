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


#include "gdf_data_block.h"

#include <QDebug>

#include <cmath>
#include <limits>

namespace SigViewer_
{

//-------------------------------------------------------------------------------------------------
GDFDataBlock::GDFDataBlock (QSharedPointer<GDFSignalCache> cache, ChannelID channel, size_t length, float64 sample_rate)
    : DataBlock (length, sample_rate),
      channel_ (channel),
      start_sample_ (0),
      current_min_ (-200),
      current_max_ (200),
      cache_ (cache)
{

}

//-------------------------------------------------------------------------------------------------
GDFDataBlock::GDFDataBlock (GDFDataBlock const& src, size_t start_sample, size_t length)
    : DataBlock (src, length),
      channel_ (src.channel_),
      start_sample_ (start_sample),
      current_min_ (src.current_min_),
      current_max_ (src.current_max_),
      downsampled_map_ (src.downsampled_map_),
      cache_ (src.cache_)
{

}


//-------------------------------------------------------------------------------------------------
GDFDataBlock::~GDFDataBlock ()
{

}

//-------------------------------------------------------------------------------------------------
QSharedPointer<DataBlock> GDFDataBlock::createSubBlock (size_t start, size_t length) const
{
    return QSharedPointer<DataBlock> (new GDFDataBlock (*this, start_sample_ + start, length));
}

//-------------------------------------------------------------------------------------------------
float32 const& GDFDataBlock::operator[] (size_t index) const
{
    current_value_ = cache_->getSample (channel_, start_sample_ + index);
    return current_value_;
}

//-------------------------------------------------------------------------
float32 GDFDataBlock::getMin () const
{
    return current_min_;
}

//-------------------------------------------------------------------------
float32 GDFDataBlock::getMax () const
{
    return current_max_;
}

//-------------------------------------------------------------------------
void GDFDataBlock::addDownSampledVersion (QSharedPointer<DataBlock> data, unsigned downsampling_factor)
{
    downsampled_map_[downsampling_factor] = data;
}

//-------------------------------------------------------------------------
std::pair<QSharedPointer<DataBlock>, unsigned> GDFDataBlock::getNearbyDownsampledBlock (unsigned downsampling_factor) const
{
    unsigned nearest_factor = 1;
    bool search = true;
    for (nearest_factor = downsampling_factor + 1; search && (nearest_factor > 1); --nearest_factor)
        if (downsampled_map_.contains (nearest_factor - 1))
            search = false;

    if ((nearest_factor <= downsampling_factor) && (nearest_factor > 1))
    {
        return std::pair<QSharedPointer<DataBlock>, unsigned> (downsampled_map_[nearest_factor]->createSubBlock(start_sample_ / nearest_factor, size() / nearest_factor), nearest_factor);
    }
    else
        return std::pair<QSharedPointer<DataBlock>, unsigned> (createSubBlock (0, size ()), 1);
}



} // namespace SigViewer_
