/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#pragma once

#include "extensionsystem_global.h"

#include <QString>
#include <QHash>
#include <QVector>

QT_BEGIN_NAMESPACE
class QStringList;
class QRegExp;
QT_END_NAMESPACE

namespace ExtensionSystem {

namespace Internal {

class OptionsParser;
class PluginSpecPrivate;
class PluginManagerPrivate;

} // Internal

class IPlugin;
class PluginView;


/*
 * 插件依赖相关信息
*/
struct EXTENSIONSYSTEM_EXPORT PluginDependency
{
    enum Type {
        Required,       // 必须有此依赖
        Optional,       // 此依赖不是必须的
        Test
    };

    PluginDependency() : type(Required) {}

    QString name;           //被依赖的插件名字
    QString version;        //版本号
    Type type;              //依赖类型
    bool operator==(const PluginDependency &other) const;
    QString toString() const;
};

uint qHash(const ExtensionSystem::PluginDependency &value);


/*
 * 插件参数描述
*/
struct EXTENSIONSYSTEM_EXPORT PluginArgumentDescription
{
    QString name;
    QString parameter;
    QString description;
};


/*
 * 核心插件类
*/
class EXTENSIONSYSTEM_EXPORT PluginSpec
{
public:
    enum State { Invalid, Read, Resolved, Loaded, Initialized, Running, Stopped, Deleted};

    ~PluginSpec();

    // information from the xml file, valid after 'Read' state is reached

    // 插件名字。当状态达到 PluginSpec::Read 时才可用。
    QString name() const;
    // 插件版本。当状态达到 PluginSpec::Read 时才可用。
    QString version() const;
    // 插件兼容版本。当状态达到 PluginSpec::Read 时才可用。
    QString compatVersion() const;

    // 插件提供者。当状态达到 PluginSpec::Read 时才可用。
    QString vendor() const;

    // 插件版权。当状态达到 PluginSpec::Read 时才可用。
    QString copyright() const;

    // 插件协议。当状态达到 PluginSpec::Read 时才可用。
    QString license() const;

    // 插件描述。当状态达到 PluginSpec::Read 时才可用。
    QString description() const;

    // 插件主页 URL。当状态达到 PluginSpec::Read 时才可用。
    QString url() const;

    // 插件类别，用于在界面分组显示插件信息。如果插件不属于任何类别，直接返回空字符串
    QString category() const;

    // 插件兼容的平台版本的正则表达式。如果兼容所有平台，则返回空。
    QRegExp platformSpecification() const;

    // 对于宿主平台是否可用。该函数用使用 platformSpecification() 的返回值对平台名字进行匹配。
    bool isAvailableForHostPlatform() const;

    // 是否必须
    bool isRequired() const;

    // 是否实验性质的插件。
    bool isHiddenByDefault() const;

    // 默认启用。实验性质的插件可能会被禁用。
    bool isExperimental() const;

    bool isEnabledByDefault() const;

    // 是否在启动时已经加载。
    bool isEnabledBySettings() const;

    bool isEffectivelyEnabled() const;

    // 因为用户取消或者因其依赖项被取消而导致该插件无法加载时，返回 true。
    bool isEnabledIndirectly() const;

    // 是否通过命令行参数 -load 加载。
    bool isForceEnabled() const;
    bool isForceDisabled() const;
    QVector<PluginDependency> dependencies() const;
    QJsonObject metaData() const;

    typedef QVector<PluginArgumentDescription> PluginArgumentDescriptions;
    // 插件处理的命令行参数描述符列表
    PluginArgumentDescriptions argumentDescriptions() const;

    // other information, valid after 'Read' state is reached

    // 该 PluginSpec 实例对应的插件 XML 描述文件所在目录的绝对位置
    QString location() const;

    // 该 PluginSpec 实例对应的插件 XML 描述文件的绝对位置（包含文件名）
    QString filePath() const;

    QStringList arguments() const;
    void setArguments(const QStringList &arguments);
    void addArgument(const QString &argument);

    bool provides(const QString &pluginName, const QString &version) const;

    // dependency specs, valid after 'Resolved' state is reached
    QHash<PluginDependency, PluginSpec *> dependencySpecs() const;
    bool requiresAny(const QSet<PluginSpec *> &plugins) const;

    // linked plugin instance, valid after 'Loaded' state is reached
    IPlugin *plugin() const;

    // state
    State state() const;
    bool hasError() const;
    QString errorString() const;

private:
    /*
     * 构造函数是私有的，显然也没有提供工厂函数进行创建.
     * 这里只能通过友元类来访问了.友元类能访问私有函数.
    */
    PluginSpec();

    Internal::PluginSpecPrivate *d;
    friend class PluginView;
    friend class Internal::OptionsParser;
    friend class Internal::PluginManagerPrivate;
    friend class Internal::PluginSpecPrivate;
};

} // namespace ExtensionSystem
