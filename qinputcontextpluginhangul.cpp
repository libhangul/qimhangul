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
    QStringList keys;
    keys += "hangul2";
    keys += "hangul3f";
    keys += "hangul39";
    keys += "hangul3s";
    keys += "hangul3y";
    keys += "hangul32";
    return keys;
}

QStringList QInputContextPluginHangul::languages( const QString &/*key*/ )
{
    return QStringList() << "ko";
}

QString QInputContextPluginHangul::displayName( const QString &key )
{
    if (key == "hangul2") {
	return "Hangul 2 bul";
    } else if (key == "hangul3f") {
	return "Hangul 3 bul final";
    } else if (key == "hangul39") {
	return "Hangul 3 bul 390";
    } else if (key == "hangul3s") {
	return "Hangul 3 bul no-shift";
    } else if (key == "hangul3y") {
	return "Hangul 3 bul yetgul";
    } else if (key == "hangul32") {
	return "Hangul 3 bul 2bul layout";
    }
    return "Hangul 2 bul";
}

QString QInputContextPluginHangul::description( const QString &/*key*/ )
{
    return "Qt input module for hangul";
}

QInputContext* QInputContextPluginHangul::create( const QString &key )
{
    QString id = key;
    id.remove(0, 6);
    return new QInputContextHangul(id.toUtf8());
}
