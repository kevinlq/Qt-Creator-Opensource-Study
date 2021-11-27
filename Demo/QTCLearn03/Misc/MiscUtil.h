#ifndef MISCUTIL_H
#define MISCUTIL_H

#include <QObject>

#if defined(MISC_LIBRARY)
#  define MISC__EXPORT Q_DECL_EXPORT
#else
#  define MISC__EXPORT Q_DECL_IMPORT
#endif

class MISC__EXPORT MiscUtil : public QObject
{
    Q_OBJECT
public:
    explicit MiscUtil(QObject *parent = nullptr);

signals:

};

#endif // MISCUTIL_H
