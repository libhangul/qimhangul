#include <qstring.h>
#include <qstringlist.h>
#include <qinputcontextplugin.h>

class QInputContextPluginHangul : public QInputContextPlugin
{
public:
    QInputContextPluginHangul();
    ~QInputContextPluginHangul();

    virtual QStringList keys() const;
    virtual QInputContext* create(const QString &key);
    virtual QStringList languages( const QString &key);
    virtual QString displayName( const QString &key );
    virtual QString description( const QString &key );
};

Q_EXPORT_PLUGIN(QInputContextPluginHangul)
