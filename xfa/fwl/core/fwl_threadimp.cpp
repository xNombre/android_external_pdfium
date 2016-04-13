// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#include "xfa/fwl/core/fwl_threadimp.h"

#include "xfa/fwl/core/fwl_appimp.h"
#include "xfa/fwl/core/fwl_noteimp.h"
#include "xfa/fwl/core/fwl_targetimp.h"
#include "xfa/fwl/core/ifwl_notethread.h"

void IFWL_Thread::Release() {
  delete m_pImpl;
}

IFWL_NoteDriver* IFWL_NoteThread::GetNoteDriver() {
  return static_cast<CFWL_NoteThreadImp*>(GetImpl())->GetNoteDriver();
}

CFWL_NoteThreadImp::CFWL_NoteThreadImp(IFWL_NoteThread* pIface)
    : CFWL_ThreadImp(pIface), m_pNoteDriver(new CFWL_NoteDriver) {}

CFWL_NoteThreadImp::~CFWL_NoteThreadImp() {
  delete m_pNoteDriver;
}

IFWL_NoteDriver* CFWL_NoteThreadImp::GetNoteDriver() {
  return m_pNoteDriver;
}
