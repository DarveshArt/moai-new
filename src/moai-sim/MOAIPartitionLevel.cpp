// Copyright (c) 2010-2017 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include "pch.h"
#include <moai-sim/MOAIPartition.h>
#include <moai-sim/MOAIPartitionLevel.h>
#include <moai-sim/MOAIPartitionHull.h>

//================================================================//
// MOAIPartitionLevel
//================================================================//

//----------------------------------------------------------------//
void MOAIPartitionLevel::Clear () {

	size_t totalCells = this->mCells.Size ();
	for ( size_t i = 0; i < totalCells; ++i ) {
		this->mCells [ i ].Clear ();
	}
}

//----------------------------------------------------------------//
void MOAIPartitionLevel::ExtractProps ( MOAIPartitionCell& cell, MOAIPartitionLevel* level ) {

	size_t totalCells = this->mCells.Size ();
	for ( size_t i = 0; i < totalCells; ++i ) {
		this->mCells [ i ].ExtractProps ( cell, level );
	}
}

//----------------------------------------------------------------//
void MOAIPartitionLevel::GatherHulls ( MOAIPartitionResultBuffer& results, MOAIPartitionHull* ignore, u32 interfaceMask, u32 queryMask ) {

	size_t totalCells = this->mCells.Size ();
	for ( size_t i = 0; i < totalCells; ++i ) {
		this->mCells [ i ].GatherHulls ( results, ignore, interfaceMask, queryMask );
	}
}

//----------------------------------------------------------------//
void MOAIPartitionLevel::GatherHulls ( MOAIPartitionResultBuffer& results, MOAIPartitionHull* ignore, const ZLVec3D& point, u32 planeID, u32 interfaceMask, u32 queryMask ) {

	ZLVec2D cellPoint ( 0.0f, 0.0f );
	
	switch ( planeID ) {
		case ZLBox::PLANE_XY:
			cellPoint.Init ( point.mX, point.mY );
			break;
		case ZLBox::PLANE_XZ:
			cellPoint.Init ( point.mX, point.mZ );
			break;
		case ZLBox::PLANE_YZ:
			cellPoint.Init ( point.mY, point.mZ );
			break;
	};

	float halfSize = this->mCellSize * 0.5f;
	cellPoint.mX = cellPoint.mX - halfSize;
	cellPoint.mY = cellPoint.mY + halfSize;

	MOAICellCoord coord = this->mGridSpace.GetCellCoord ( cellPoint.mX, cellPoint.mY );
	
	int width = this->mGridSpace.GetWidth ();
	int height = this->mGridSpace.GetHeight ();
	
	int xTotal = ( width < 2 ) ? 1 : 2;
	int yTotal = ( height < 2 ) ? 1 : 2;
	
	for ( int y = 0; y < yTotal; ++y ) {
		for ( int x = 0; x < xTotal; ++x ) {
			
			MOAICellCoord offset = this->mGridSpace.WrapCellCoord ( coord.mX + x, coord.mY - y );
			int addr = this->mGridSpace.GetCellAddr ( offset );
			this->mCells [ addr ].GatherHulls ( results, ignore, point, interfaceMask, queryMask );
		}
	}
}

//----------------------------------------------------------------//
void MOAIPartitionLevel::GatherHulls ( MOAIPartitionResultBuffer& results, MOAIPartitionHull* ignore, const ZLVec3D& point, const ZLVec3D& orientation, u32 interfaceMask, u32 queryMask ) {
	
	// TODO: this is so lazy; fix it to use the plane and step through the proper cells
	size_t totalCells = this->mCells.Size ();
	for ( size_t i = 0; i < totalCells; ++i ) {
		this->mCells [ i ].GatherHulls ( results, ignore, point, orientation, interfaceMask, queryMask );
	}
}

//----------------------------------------------------------------//
void MOAIPartitionLevel::GatherHulls ( MOAIPartitionResultBuffer& results, MOAIPartitionHull* ignore, const ZLRect& rect, u32 interfaceMask, u32 queryMask ) {

	float halfSize = this->mCellSize * 0.5f;

	MOAICellCoord coord0 = this->mGridSpace.GetCellCoord ( rect.mXMin - halfSize, rect.mYMin - halfSize );
	MOAICellCoord coord1 = this->mGridSpace.GetCellCoord ( rect.mXMax + halfSize, rect.mYMax + halfSize );

	int xTotal = coord1.mX - coord0.mX + 1;
	int yTotal = coord1.mY - coord0.mY + 1;
	
	int width = this->mGridSpace.GetWidth ();
	int height = this->mGridSpace.GetHeight ();
	
	if ( xTotal > width ) xTotal = width;
	if ( yTotal > height ) yTotal = height;

	for ( int y = 0; y < yTotal; ++y ) {
		for ( int x = 0; x < xTotal; ++x ) {
			
			MOAICellCoord offset = this->mGridSpace.WrapCellCoord ( coord0.mX + x, coord0.mY + y );
			u32 addr = this->mGridSpace.GetCellAddr ( offset );
			this->mCells [ addr ].GatherHulls ( results, ignore, rect, interfaceMask, queryMask );
		}
	}
}

//----------------------------------------------------------------//
void MOAIPartitionLevel::GatherHulls ( MOAIPartitionResultBuffer& results, MOAIPartitionHull* ignore, const ZLBox& box, u32 planeID, u32 interfaceMask, u32 queryMask ) {

	float halfSize = this->mCellSize * 0.5f;

	ZLRect rect = box.GetRect ( planeID );
	MOAICellCoord coord0 = this->mGridSpace.GetCellCoord ( rect.mXMin - halfSize, rect.mYMin - halfSize );
	MOAICellCoord coord1 = this->mGridSpace.GetCellCoord ( rect.mXMax + halfSize, rect.mYMax + halfSize );

	int xTotal = coord1.mX - coord0.mX + 1;
	int yTotal = coord1.mY - coord0.mY + 1;
	
	int width = this->mGridSpace.GetWidth ();
	int height = this->mGridSpace.GetHeight ();
	
	if ( xTotal > width ) xTotal = width;
	if ( yTotal > height ) yTotal = height;

	for ( int y = 0; y < yTotal; ++y ) {
		for ( int x = 0; x < xTotal; ++x ) {
			
			MOAICellCoord offset = this->mGridSpace.WrapCellCoord ( coord0.mX + x, coord0.mY + y );
			u32 addr = this->mGridSpace.GetCellAddr ( offset );
			this->mCells [ addr ].GatherHulls ( results, ignore, box, interfaceMask, queryMask );
		}
	}
}

//----------------------------------------------------------------//
void MOAIPartitionLevel::GatherHulls ( MOAIPartitionResultBuffer& results, MOAIPartitionHull* ignore, const ZLFrustum& frustum, u32 planeID, u32 interfaceMask, u32 queryMask ) {

	float halfSize = this->mCellSize * 0.5f;

	ZLRect rect = frustum.mAABB.GetRect ( planeID );
	MOAICellCoord coord0 = this->mGridSpace.GetCellCoord ( rect.mXMin - halfSize, rect.mYMin - halfSize );
	MOAICellCoord coord1 = this->mGridSpace.GetCellCoord ( rect.mXMax + halfSize, rect.mYMax + halfSize );

	int xTotal = coord1.mX - coord0.mX + 1;
	int yTotal = coord1.mY - coord0.mY + 1;
	
	int width = this->mGridSpace.GetWidth ();
	int height = this->mGridSpace.GetHeight ();
	
	if ( xTotal > width ) xTotal = width;
	if ( yTotal > height ) yTotal = height;

	for ( int y = 0; y < yTotal; ++y ) {
		for ( int x = 0; x < xTotal; ++x ) {
			
			MOAICellCoord offset = this->mGridSpace.WrapCellCoord ( coord0.mX + x, coord0.mY + y );
			u32 addr = this->mGridSpace.GetCellAddr ( offset );
			this->mCells [ addr ].GatherHulls ( results, ignore, frustum, interfaceMask, queryMask );
		}
	}
}

//----------------------------------------------------------------//
void MOAIPartitionLevel::Init ( float cellSize, u32 width, u32 height ) {

	this->mCellSize = cellSize;

	this->mGridSpace.SetWidth ( width );
	this->mGridSpace.SetHeight ( height );
	this->mGridSpace.SetCellWidth ( cellSize );
	this->mGridSpace.SetCellHeight ( cellSize );
	
	this->mCells.Init ( this->mGridSpace.GetTotalCells ());
}

//----------------------------------------------------------------//
MOAIPartitionCell* MOAIPartitionLevel::GetCell ( MOAIPartitionHull& hull ) {

	ZLVec3D loc;
	hull.mWorldBounds.GetCenter ( loc );
	
	MOAICellCoord coord = this->mGridSpace.GetCellCoord ( loc.mX, loc.mY );
	u32 cellAddr = this->mGridSpace.GetCellAddr ( coord );
	return &this->mCells [ cellAddr ];
}

//----------------------------------------------------------------//
MOAIPartitionLevel::MOAIPartitionLevel () :
	mCellSize ( 0.0f ) {
}

//----------------------------------------------------------------//
MOAIPartitionLevel::~MOAIPartitionLevel () {
	this->Clear ();
}

//----------------------------------------------------------------//
void MOAIPartitionLevel::PlaceHull ( MOAIPartitionHull& hull ) {

	MOAIPartitionCell* cell = this->GetCell ( hull );
	cell->InsertHull ( hull );
}
