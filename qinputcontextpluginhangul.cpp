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
    qDebug("QInputContextPluginHangul::");
}

QInputContextPluginHangul::~QInputContextPluginHangul()
{
    qDebug("QInputContextPluginHangul::~");
}

QStringList QInputContextPluginHangul::keys() const
{
    qDebug("QInputContextPluginHangul::keys()");
    QStringList keys;
    keys += "Hangul2";
    keys += "Hangul3f";
    keys += "Hangul390";
    keys += "Hangul32";
    return keys;
}

QStringList QInputContextPluginHangul::languages( const QString &key )
{
    qDebug("QInputContextPluginHangul::languages()" + key);
    return QStringList() << "ko";
}

QString QInputContextPluginHangul::displayName( const QString &key )
{
    if (key == "Hangul2") {
	return "Hangul 2 bul";
    } else if (key == "Hangul3f") {
	return "Hangul 3 bul final";
    } else if (key == "Hangul390") {
	return "Hangul 3 bul 390";
    } else if (key == "Hangul32") {
	return "Hangul 3 bul 2bul layout";
    }
    return "Hangul 2 bul";
}

QString QInputContextPluginHangul::description( const QString &key )
{
    qDebug("QInputContextPluginHangul::description()" + key);
    return "Qt input module for hangul";
}

QInputContext* QInputContextPluginHangul::create( const QString &key )
{
    if (key == "Hangul2") {
	return new QInputContextHangul(hangul::ComposerBase::HANGUL2);
    } else if (key == "Hangul3f") {
	return new QInputContextHangul(hangul::ComposerBase::HANGUL3F);
    } else if (key == "Hangul390") {
	return new QInputContextHangul(hangul::ComposerBase::HANGUL390);
    } else if (key == "Hangul32") {
	return new QInputContextHangul(hangul::ComposerBase::HANGUL32);
    } else {
	return new QInputContextHangul(hangul::ComposerBase::HANGUL2);
    }

    qDebug("QInputContextPluginHangul::create() - " + key);
    return 0;
}
