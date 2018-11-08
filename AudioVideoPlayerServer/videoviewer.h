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
 * videoviewer.h
 *
 *  Created on: Feb 17, 2018
 *      Author: Rebeca Bernstein
 */

#ifndef VIDEOVIEWER_H
#define VIDEOVIEWER_H

#include <QMainWindow>
#include <QPrinter>
#include <vector>
#include <queue>
#include "processor.h"

class QAction;
class QLabel;
class QMenu;

/**
 * @brief The VideoViewer class represnt a Qt GUI window used to display
 * the video feed for a real-time one-way video conferencing application.
 */
class VideoViewer : public QMainWindow
{
    Q_OBJECT

public:
    VideoViewer();

    std::vector<Processor*>* processes;

    void emitImage(std::vector<unsigned char>* data);

signals:
    /**
     * @brief signalRefreshImage - the event emitted when there is a new video frame to display
     * @param data - the video frame
     */
    void signalRefreshImage(std::vector<unsigned char>* data);

protected:
    virtual void closeEvent(QCloseEvent *event);

private slots:
    void about();
    void closeConnection();
    void refreshImage(std::vector<unsigned char>* data);

private:
    void createActions();
    void createMenus();

    /**
     * @brief videoFrame - image display to be updated with video frames
     */
    QLabel *videoFrame;


#ifndef QT_NO_PRINTER
    QPrinter printer;
#endif

    /**
     * @brief exitAct - closes the application
     */
    QAction *exitAct;

    /**
     * @brief aboutAct - displays a dialog about the application
     */
    QAction *aboutAct;

    /**
     * @brief aboutQtAct - displays a dialog about Qt
     */
    QAction *aboutQtAct;

    QMenu *fileMenu;
    QMenu *helpMenu;
};

#endif
