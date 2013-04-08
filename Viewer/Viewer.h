//------------------------------------------------------------------------------
// <copyright file="ColorBasics.h" company="Microsoft">
//     Copyright (c) Microsoft Corporation.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

#pragma once

#include "resource.h"
#include "NuiApi.h"
#include "ImageRenderer.h"
#include "TrackerCore.h"

class Viewer
{
    static const int        cColorWidth  = 640;
    static const int        cColorHeight = 480;

    static const int        cStatusMessageMaxLen = MAX_PATH*2;

public:
    /// <summary>
    /// Constructor
    /// </summary>
    Viewer();

    /// <summary>
    /// Destructor
    /// </summary>
    ~Viewer();

    /// <summary>
    /// Handles window messages, passes most to the class instance to handle
    /// </summary>
    /// <param name="hWnd">window message is for</param>
    /// <param name="uMsg">message</param>
    /// <param name="wParam">message data</param>
    /// <param name="lParam">additional message data</param>
    /// <returns>result of message processing</returns>
    static LRESULT CALLBACK MessageRouter(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    /// <summary>
    /// Handle windows messages for a class instance
    /// </summary>
    /// <param name="hWnd">window message is for</param>
    /// <param name="uMsg">message</param>
    /// <param name="wParam">message data</param>
    /// <param name="lParam">additional message data</param>
    /// <returns>result of message processing</returns>
    LRESULT CALLBACK        DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    /// <summary>
    /// Creates the main window and begins processing
    /// </summary>
    /// <param name="hInstance"></param>
    /// <param name="nCmdShow"></param>
    int                     Run(HINSTANCE hInstance, int nCmdShow);

private:
    HWND                    m_hWnd;

    // Current Kinect
    INuiSensor*             m_pNuiSensor;

    // Direct2D
    ImageRenderer*          m_pDrawColor;
    ID2D1Factory*           m_pD2DFactory;

	// Local Tracker Core
	TrackerCore*			m_pTrackerCore;
    HANDLE                  m_hNextDepthFrameEvent;
    HANDLE                  m_pDepthStreamHandle;
    HANDLE                  m_pColorStreamHandle;
    HANDLE                  m_hNextColorFrameEvent;

	// for mapping depth to color
    USHORT*					m_depthD16;
    BYTE*					m_colorRGBX;
    LONG*					m_colorCoordinates;

    // to prevent use until we have data for both streams
    bool					m_bDepthReceived;
    bool					m_bColorReceived;

    /// <summary>
    /// Main processing function
    /// </summary>
    void                    Update();

    /// <summary>
    /// Create the first connected Kinect found 
    /// </summary>
    /// <returns>S_OK on success, otherwise failure code</returns>
    HRESULT                 CreateFirstConnected();

    /// <summary>
    /// Process depth data received from Kinect
    /// </summary>
    /// <returns>S_OK for success, or failure code</returns>
    HRESULT                             ProcessDepth();

    /// <summary>
    /// Process color data received from Kinect
    /// </summary>
    /// <returns>S_OK for success, or failure code</returns>
    HRESULT                             ProcessColor();

	void CheackDepth();
	int SendMessager();
};
