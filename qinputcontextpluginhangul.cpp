/* qimhangul - Qt input module for hangul
 * Copyright (C) 2004 Choe Hwanjin
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#include "qinputcontextpluginhangul.h"
#include "qinputcontexthangul.h"

#include <QDebug>

QInputContextPluginHangul::QInputContextPluginHangul(QObject* parent/*= nullptr*/) :
    QPlatformInputContextPlugin(parent)
{
    if (QInputContextHangul::hanjaTable)
	hanja_table_delete(QInputContextHangul::hanjaTable);
    QInputContextHangul::hanjaTable = hanja_table_load(NULL);
}

QInputContextPluginHangul::~QInputContextPluginHangul()
{
    if (QInputContextHangul::hanjaTable)
	hanja_table_delete(QInputContextHangul::hanjaTable);
}

QPlatformInputContext*
QInputContextPluginHangul::create(const QString& /*key*/, const QStringList& paramList)
{
    return new QInputContextHangul(paramList);
}
