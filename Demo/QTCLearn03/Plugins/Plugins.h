#ifndef PLUGINS_H
#define PLUGINS_H

#include <QObject>

#if defined(PLUGIN_LIBRARY)
#  define PLUGIN__EXPORT Q_DECL_EXPORT
#else
#  define PLUGIN__EXPORT Q_DECL_IMPORT
#endif

class PLUGIN__EXPORT Plugins : public QObject
{
    Q_OBJECT
public:
    explicit Plugins(QObject *parent = nullptr);

};

#endif // PLUGINS_H
