/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2017 Néstor Silveira Gorski

-------------------------------------------------------------------------------
This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#include "QtQuickItem.h"
#include "NSG.h"
#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLShaderProgram>
#include <QtQuick/qquickwindow.h>
using namespace NSG;

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

QtQuickItemRenderer::QtQuickItemRenderer(QQuickWindow* qtWindow)
    : qtWindow_(qtWindow), initialized_(false) {}

QtQuickItemRenderer::~QtQuickItemRenderer() {}

void QtQuickItemRenderer::Paint() {
    auto window = Window::GetMainWindow();
    if (window) {
        if (!initialized_) {
            CHECK_CONDITION(window->IsReady());
            initialized_ = true;
            initializeOpenGLFunctions();
        }
        window->ViewChanged(qtWindow_->size().width(),
                            qtWindow_->size().height());
        RenderingContext::GetPtr()->ResetCachedState();
        Engine::GetPtr()->PerformTicks();
        qtWindow_->resetOpenGLState();
    }
}

////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
QtQuickItem::QtQuickItem() {
    connect(this, &QQuickItem::windowChanged, this,
            &QtQuickItem::HandleWindowChanged);
    timer_.start(12, this);
}

void QtQuickItem::HandleWindowChanged(QQuickWindow* win) {
    if (win) {
        connect(win, &QQuickWindow::beforeSynchronizing, this,
                &QtQuickItem::Sync, Qt::DirectConnection);
        connect(win, &QQuickWindow::sceneGraphInvalidated, this,
                &QtQuickItem::Cleanup, Qt::DirectConnection);
        // If we allow QML to do the clearing, they would clear what we paint
        // and nothing would show.
        win->setClearBeforeRendering(false);
    }
}

void QtQuickItem::Cleanup() { renderer_ = nullptr; }

void QtQuickItem::Sync() {
    if (!renderer_) {
        renderer_ = std::unique_ptr<QtQuickItemRenderer>(
            new QtQuickItemRenderer(window()));
        connect(window(), &QQuickWindow::beforeRendering, renderer_.get(),
                &QtQuickItemRenderer::Paint, Qt::DirectConnection);
    }
}

void QtQuickItem::timerEvent(QTimerEvent*) { window()->update(); }
