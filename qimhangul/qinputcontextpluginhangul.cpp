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
