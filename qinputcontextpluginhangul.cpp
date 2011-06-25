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

#include "qinputcontexthangul.h"
#include "qinputcontextpluginhangul.h"

QInputContextPluginHangul::QInputContextPluginHangul()
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

QStringList QInputContextPluginHangul::keys() const
{
    int n = hangul_ic_get_n_keyboards();

    QStringList keys;

    for (int i = 0; i < n; ++i) {
	const char* id = hangul_ic_get_keyboard_id(i);

	keys += QString("hangul").append(id);
    }

    return keys;
}

QStringList QInputContextPluginHangul::languages( const QString &/*key*/ )
{
    return QStringList() << "ko";
}

QString QInputContextPluginHangul::displayName( const QString &key )
{
    int i;
    int n;
    const char* name;

    QString id = key.mid(sizeof("hangul") - 1, -1);

    n = hangul_ic_get_n_keyboards();
    for (i = 0; i < n; ++i) {
	const char* s = hangul_ic_get_keyboard_id(i);

	if (id == s) {
	    name = hangul_ic_get_keyboard_name(i);
	    return QString::fromUtf8(name);
	}
    }

    return "";
}

QString QInputContextPluginHangul::description( const QString &/*key*/ )
{
    return "Qt input module for hangul";
}

QInputContext* QInputContextPluginHangul::create( const QString &key )
{
    QString id = key.mid(sizeof("hangul") - 1, -1);
    return new QInputContextHangul(id.toUtf8());
}
