/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file tile_pane.h
 * @date 2021-11-28
 * 
 * @copyright Copyright (c) 2021
 */

#pragma once

#include "container.h"

namespace Sgl
{
    class TilePane : public Container
    {
    public:
        TilePane(int32_t prefColumns = 1);
        virtual ~TilePane() override = default;

        int32_t getTiles() const;

        int32_t getPrefColumns() const;
        void setPrefColumns(int32_t columns);

        int32_t getPrefTileWidth() const;
        void setPrefTileWidth(int32_t width);

        int32_t getPrefTileHeight() const;
        void setPrefTileHeight(int32_t height);

        int32_t getColumns() const;
        int32_t getRows() const;
        int32_t getTileWidth() const;
        int32_t getTileHeight() const;

        int32_t getHgap() const;
        void setHgap(int32_t gap);

        int32_t getVgap() const;
        void setVgap(int32_t gap);

    private:
        int32_t m_PrefColumns    = 0;

        int32_t m_PrefTileWidth  = USE_COMPUTED_SIZE;
        int32_t m_PrefTileHeight = USE_COMPUTED_SIZE;

        int32_t m_Columns        = 0;
        int32_t m_Rows           = 0;
        int32_t m_TileWidth      = 0;
        int32_t m_TileHeight     = 0;

        int32_t m_Hgap           = 0;
        int32_t m_Vgap           = 0;

    private:
        virtual void layoutChildren() override;

        virtual int32_t computeCustomPrefWidth(int32_t height = -1) const override;
        virtual int32_t computeCustomPrefHeight(int32_t width = -1) const override;
        
        virtual int32_t computeCustomMinWidth(int32_t height = -1) const override;
        virtual int32_t computeCustomMinHeight(int32_t width = -1) const override;

    private:
        int32_t computeTileWidth() const;
        int32_t computeTileHeight() const;
        int32_t computeColumns(int32_t width) const;
        int32_t computeRows() const;
    };
}