/**
 * @author Nikita Mochalov (github.com/tralf-strues)
 * @file tile_pane.cpp
 * @date 2021-11-28
 * 
 * @copyright Copyright (c) 2021
 */

#include "scene/containers/tile_pane.h"

namespace Sgl
{
    TilePane::TilePane(int32_t prefColumns) : m_PrefColumns(prefColumns) { assert(prefColumns > 0); }

    int32_t TilePane::getTiles() const               { return static_cast<int32_t>(m_Children.size()); }

    int32_t TilePane::getPrefColumns() const         { return m_PrefColumns;    }
    void TilePane::setPrefColumns(int32_t columns)   { m_PrefColumns = columns; }

    int32_t TilePane::getPrefTileWidth() const       { return m_PrefTileWidth;  }
    void TilePane::setPrefTileWidth(int32_t width)   { m_PrefTileWidth = width; }

    int32_t TilePane::getPrefTileHeight() const      { return m_PrefTileHeight;   }
    void TilePane::setPrefTileHeight(int32_t height) { m_PrefTileHeight = height; }

    int32_t TilePane::getColumns() const             { return m_Columns;    }
    int32_t TilePane::getRows() const                { return m_Rows;       }
    int32_t TilePane::getTileWidth() const           { return m_TileWidth;  }
    int32_t TilePane::getTileHeight() const          { return m_TileHeight; }

    int32_t TilePane::getHgap() const                { return m_Hgap; }
    void TilePane::setHgap(int32_t gap)              { m_Hgap = gap;  }

    int32_t TilePane::getVgap() const                { return m_Vgap; }
    void TilePane::setVgap(int32_t gap)              { m_Vgap = gap;  }

    void TilePane::layoutChildren()
    {
        int32_t tiles = static_cast<int32_t>(m_Children.size());
        if (tiles == 0) { return; }

        Sml::Rectangle<int32_t> contentArea = getContentArea();

        m_TileWidth  = computeTileWidth();
        m_TileHeight = computeTileHeight();
        m_Columns    = computeColumns(contentArea.width);
        m_Rows       = computeRows();

        int32_t curY = contentArea.pos.y;
        auto    it   = m_Children.begin();

        for (int32_t row = 0; row < m_Rows; ++row)
        {
            int32_t curX = contentArea.pos.x;
            for (int32_t column = 0; column < m_Columns && (it != m_Children.end()); ++column)
            {
                Component* child = (*it);
                
                child->setLayoutX(curX);
                child->setLayoutY(curY);
                child->setLayoutWidth(m_TileWidth);
                child->setLayoutHeight(m_TileHeight);

                curX += m_TileWidth + m_Hgap;
                ++it;
            }

            curY += m_TileHeight + m_Vgap;
        }
    }

    int32_t TilePane::computeCustomPrefWidth(int32_t height) const
    {
        int32_t columns = getPrefColumns();

        int32_t prefWidth = getInsets().left + getInsets().right;
        prefWidth += columns * computeTileWidth();
        prefWidth += (columns - 1) * getHgap();

        return prefWidth;
    }

    int32_t TilePane::computeCustomPrefHeight(int32_t width) const
    {
        int32_t rows = computeRows();

        int32_t prefHeight = getInsets().top + getInsets().bottom;
        prefHeight += rows * computeTileHeight();
        prefHeight += (rows - 1) * getVgap();

        return prefHeight;
    }
    
    int32_t TilePane::computeCustomMinWidth(int32_t height) const
    {
        return getInsets().left + getInsets().right + computeTileWidth();
    }

    int32_t TilePane::computeCustomMinHeight(int32_t width) const
    {
        return getInsets().top + getInsets().bottom + computeTileHeight();
    }

    int32_t TilePane::computeTileWidth() const
    {
        if (m_PrefTileWidth != USE_COMPUTED_SIZE)
        {
            return m_PrefTileWidth;
        }

        int32_t maxPrefWidth = 0;
        for (auto child : getChildrenReadonly())
        {
            int32_t prefWidth = child->computePrefWidth();

            if (prefWidth > maxPrefWidth)
            {
                maxPrefWidth = prefWidth;
            }
        }

        return maxPrefWidth;
    }

    int32_t TilePane::computeTileHeight() const
    {
        if (m_PrefTileHeight != USE_COMPUTED_SIZE)
        {
            return m_PrefTileHeight;
        }

        int32_t maxPrefHeight = 0;
        for (auto child : getChildrenReadonly())
        {
            int32_t prefHeight = child->computePrefHeight();

            if (prefHeight > maxPrefHeight)
            {
                maxPrefHeight = prefHeight;
            }
        }

        return maxPrefHeight;
    }

    int32_t TilePane::computeColumns(int32_t width) const
    {
        if (getTileWidth() + getHgap() <= 0)
        {
            return 0;
        }

        return (width + getHgap()) / (getTileWidth() + getHgap());
    }

    int32_t TilePane::computeRows() const
    {
        return (getTiles() - 1) / getColumns() + 1;
    }
}