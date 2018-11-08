/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

/*
 * videoviewer.cpp
 *
 *  Created on: Feb 17, 2018
 *      Author: Rebecca Bernstein
 */

#include <QtGui>
#include <iostream>

#include "videoviewer.h"

/**
 * @brief VideoViewer::VideoViewer Constructs the GUI for a real-time video feed
 */
VideoViewer::VideoViewer()
{
    videoFrame = new QLabel;
    videoFrame->setBackgroundRole(QPalette::Base);
    videoFrame->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    videoFrame->setScaledContents(true);
    videoFrame->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    setCentralWidget(videoFrame);

    createActions();
    createMenus();

    setWindowTitle(tr("Video Viewer - Real-time video feed"));
    // resize(500, 400);
}

/**
 * @brief VideoViewer::refreshImage Qt slot loads the given data as a JPG img and updates a QLabel
 * @param data - a JPG img frame
 */
void VideoViewer::refreshImage(std::vector<unsigned char>* data)
{
    QPixmap img;
    img.loadFromData(&(*data)[0], data->size(), "JPG");
    videoFrame->setPixmap(img);
}

/**
 * @brief VideoViewer::emitImage emits a Qt signal to update the video display
 * @param data - a JPG img frame
 */
void VideoViewer::emitImage(std::vector<unsigned char>* data)
{
    emit signalRefreshImage(data);
}

/**
 * @brief VideoViewer::closeConnection called when the user closes the GUI
 */
void VideoViewer::closeConnection()
{
    // These operations should be defined elsewhere
    // It is convenient to do here for now

    std::cout << "Closing connection..." << std::endl;
    Processor* process;
    while(processes->size() > 0) {
        process = processes->back();
        processes->pop_back();
        process->stop();
        std::cout << "Deleting a process..." << std::endl;
        delete process;
        std::cout << "\tprocess deleted." << std::endl;
    }

    std::cout << "...Connection closed." << std::endl;
}

/**
 * @brief VideoViewer::about display info about this application
 */
void VideoViewer::about()
{
    QMessageBox::about(this, tr("About Image Viewer"),
            tr("<p>The <b>Video Viewer</b> displays a real-time video feed "
               "using a QLabel.  The QLabel must be updated from within the Qt "
               "UI thread, so a signal is emitted when new data arrives. </p>"
               "<p>Processes are gracefully shut down when the exit action is"
               " called.</p>"));
}

/**
 * @brief VideoViewer::createActions creates actions.  Connects action-triggers
 * and other signals to slots.
 */
void VideoViewer::createActions()
{
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    // connect(this, SIGNAL(aboutToQuit()), this, SLOT(closeConnection()));

    connect(this, SIGNAL(signalRefreshImage(std::vector<unsigned char>*)),
                         this, SLOT(refreshImage(std::vector<unsigned char>*)));
}

/**
 * @brief VideoViewer::createMenus creates the top menu bar
 */
void VideoViewer::createMenus()
{
    fileMenu = new QMenu(tr("&File"), this);
    // fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(helpMenu);
}

/**
 * @brief VideoViewer::closeEvent calls closeConnection before passing the close event to its parent
 * @param event
 */
void VideoViewer::closeEvent(QCloseEvent *event)
{
    closeConnection();
    QMainWindow::closeEvent(event);
}
