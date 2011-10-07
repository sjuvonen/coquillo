/***********************************************************************
* Copyright (c) 2011 Samu Juvonen <samu.juvonen@gmail.com>
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
************************************************************************/

#include <QApplication>
#include <QModelIndex>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QStyleOptionViewItemV4>

#include "PictureListDelegate.h"

#include <QDebug>

void PictureListDelegate::paint(QPainter * painter, const QStyleOptionViewItem & opt, const QModelIndex & index) const {
	enum { PictureTypeRole = Qt::UserRole+1, PictureTypeStringRole, PictureDimensionsRole };

	QStyleOptionViewItemV4 option = opt;
	initStyleOption(&option, index);

	int pad = 4;
	const QRect r = option.rect;

	QString text = option.text;
	option.text.clear();

	const QWidget * widget = option.widget;
	const QPalette palette = widget ? widget->palette() : QApplication::palette();
	QStyle * style = widget ? widget->style() : QApplication::style();

  style->drawControl(QStyle::CE_ItemViewItem, &option, painter, widget);

	painter->save();
	painter->translate(option.rect.x(), option.rect.y());

	if (option.state & QStyle::State_Selected)
		painter->setPen(palette.color(QPalette::BrightText));

	int rowHeight = QFontMetrics(option.font).size(0, "Foo").height() + 2;
	int y = (r.height() - 3*rowHeight - pad - pad) / 2;
	int w = option.decorationSize.width() + 2*pad + 5;

	const QSize s = index.data(PictureDimensionsRole).toSize();
	QRect rowRect( w, y, r.width() - w, rowHeight );

	style->drawItemText(painter, rowRect, Qt::AlignLeft, palette, true, text);

	rowRect.moveTop( rowRect.top() + rowHeight );
	text = index.data(PictureTypeStringRole).toString();
	style->drawItemText(painter, rowRect, Qt::AlignLeft, palette, true, text);

	rowRect.moveTop( rowRect.top() + rowHeight );
	text = QString("%1 x %2 px").arg(s.width()).arg(s.height());
	style->drawItemText(painter, rowRect, Qt::AlignLeft, palette, true, text);

	painter->restore();
}