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

#include "autotestplugin.h"
#include "autotestconstants.h"
#include "autotesticons.h"
#include "testcodeparser.h"
#include "testframeworkmanager.h"
#include "testrunner.h"
#include "testsettings.h"
#include "testsettingspage.h"
#include "testtreeitem.h"
#include "testtreeview.h"
#include "testtreemodel.h"
#include "testresultspane.h"
#include "testnavigationwidget.h"

#include "qtest/qttestframework.h"
#include "quick/quicktestframework.h"
#include "gtest/gtestframework.h"

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/coreconstants.h>
#include <extensionsystem/pluginmanager.h>
#include <projectexplorer/buildmanager.h>
#include <projectexplorer/projectexplorer.h>
#include <projectexplorer/session.h>
#include <projectexplorer/target.h>
#include <utils/utilsicons.h>

#include <QAction>
#include <QMessageBox>
#include <QMainWindow>
#include <QMenu>

#include <QtPlugin>

#ifdef WITH_TESTS
#include "autotestunittests.h"
#endif

using namespace Autotest::Internal;
using namespace Core;

static AutotestPlugin *s_instance = nullptr;

AutotestPlugin::AutotestPlugin()
    : m_settings(new TestSettings)
{
    // needed to be used in QueuedConnection connects
    qRegisterMetaType<TestResult>();
    qRegisterMetaType<TestTreeItem *>();
    qRegisterMetaType<TestCodeLocationAndType>();

    s_instance = this;
}

AutotestPlugin::~AutotestPlugin()
{
    delete m_frameworkManager;
}

AutotestPlugin *AutotestPlugin::instance()
{
    return s_instance;
}

QSharedPointer<TestSettings> AutotestPlugin::settings() const
{
    return m_settings;
}

void AutotestPlugin::initializeMenuEntries()
{
    ActionContainer *menu = ActionManager::createMenu(Constants::MENU_ID);
    menu->menu()->setTitle(tr("&Tests"));
    menu->setOnAllDisabledBehavior(ActionContainer::Show);

    QAction *action = new QAction(tr("Run &All Tests"), this);
    action->setIcon(Utils::Icons::RUN_SMALL_TOOLBAR.icon());
    action->setToolTip(tr("Run All Tests"));
    Command *command = ActionManager::registerAction(action, Constants::ACTION_RUN_ALL_ID);
    command->setDefaultKeySequence(QKeySequence(tr("Alt+Shift+T,Alt+A")));
    connect(action, &QAction::triggered, this, &AutotestPlugin::onRunAllTriggered);
    action->setEnabled(false);
    menu->addAction(command);

    action = new QAction(tr("&Run Selected Tests"), this);
    Utils::Icon runSelectedIcon = Utils::Icons::RUN_SMALL_TOOLBAR;
    for (const Utils::IconMaskAndColor &maskAndColor : Icons::RUN_SELECTED_OVERLAY)
        runSelectedIcon.append(maskAndColor);
    action->setIcon(runSelectedIcon.icon());
    action->setToolTip(tr("Run Selected Tests"));
    command = ActionManager::registerAction(action, Constants::ACTION_RUN_SELECTED_ID);
    command->setDefaultKeySequence(QKeySequence(tr("Alt+Shift+T,Alt+R")));
    connect(action, &QAction::triggered, this, &AutotestPlugin::onRunSelectedTriggered);
    action->setEnabled(false);
    menu->addAction(command);

    action = new QAction(tr("Re&scan Tests"), this);
    command = ActionManager::registerAction(action, Constants::ACTION_SCAN_ID);
    command->setDefaultKeySequence(QKeySequence(tr("Alt+Shift+T,Alt+S")));
    connect(action, &QAction::triggered, this, [] () {
        TestTreeModel::instance()->parser()->updateTestTree();
    });
    menu->addAction(command);

    ActionContainer *toolsMenu = ActionManager::actionContainer(Core::Constants::M_TOOLS);
    toolsMenu->addMenu(menu);
    using namespace ProjectExplorer;
    connect(BuildManager::instance(), &BuildManager::buildStateChanged,
            this, &AutotestPlugin::updateMenuItemsEnabledState);
    connect(BuildManager::instance(), &BuildManager::buildQueueFinished,
            this, &AutotestPlugin::updateMenuItemsEnabledState);
    connect(ProjectExplorerPlugin::instance(), &ProjectExplorerPlugin::updateRunActions,
            this, &AutotestPlugin::updateMenuItemsEnabledState);
    connect(TestTreeModel::instance(), &TestTreeModel::testTreeModelChanged,
            this, &AutotestPlugin::updateMenuItemsEnabledState);
}

bool AutotestPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    m_frameworkManager = TestFrameworkManager::instance();
    initializeMenuEntries();
    m_frameworkManager->registerTestFramework(new QtTestFramework);
    m_frameworkManager->registerTestFramework(new QuickTestFramework);
    m_frameworkManager->registerTestFramework(new GTestFramework);

    m_frameworkManager->synchronizeSettings(ICore::settings());
    addAutoReleasedObject(new TestSettingsPage(m_settings));
    addAutoReleasedObject(new TestNavigationWidgetFactory);
    addAutoReleasedObject(TestResultsPane::instance());

    m_frameworkManager->activateFrameworksFromSettings(m_settings);
    TestTreeModel::instance()->syncTestFrameworks();

    return true;
}

void AutotestPlugin::extensionsInitialized()
{
}

ExtensionSystem::IPlugin::ShutdownFlag AutotestPlugin::aboutToShutdown()
{
    TestTreeModel::instance()->parser()->aboutToShutdown();
    return SynchronousShutdown;
}

void AutotestPlugin::onRunAllTriggered()
{
    TestRunner *runner = TestRunner::instance();
    TestTreeModel *model = TestTreeModel::instance();
    runner->setSelectedTests(model->getAllTestCases());
    runner->prepareToRunTests(TestRunMode::Run);
}

void AutotestPlugin::onRunSelectedTriggered()
{
    TestRunner *runner = TestRunner::instance();
    TestTreeModel *model = TestTreeModel::instance();
    runner->setSelectedTests(model->getSelectedTests());
    runner->prepareToRunTests(TestRunMode::Run);
}

void AutotestPlugin::updateMenuItemsEnabledState()
{
    const ProjectExplorer::Project *project = ProjectExplorer::SessionManager::startupProject();
    const ProjectExplorer::Target *target = project ? project->activeTarget() : nullptr;
    const bool canScan = !TestRunner::instance()->isTestRunning()
            && TestTreeModel::instance()->parser()->state() == TestCodeParser::Idle;
    const bool hasTests = TestTreeModel::instance()->hasTests();
    // avoid expensive call to PE::canRunStartupProject() - limit to minimum necessary checks
    const bool canRun = hasTests && canScan
            && project && !project->needsConfiguration()
            && target && target->activeRunConfiguration()
            && !ProjectExplorer::BuildManager::isBuilding();

    ActionManager::command(Constants::ACTION_RUN_ALL_ID)->action()->setEnabled(canRun);
    ActionManager::command(Constants::ACTION_RUN_SELECTED_ID)->action()->setEnabled(canRun);
    ActionManager::command(Constants::ACTION_SCAN_ID)->action()->setEnabled(canScan);
}

QList<QObject *> AutotestPlugin::createTestObjects() const
{
    QList<QObject *> tests;
#ifdef WITH_TESTS
    tests << new AutoTestUnitTests(TestTreeModel::instance());
#endif
    return tests;
}