// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef FPDFSDK_FPDFXFA_CPDFXFA_CONTEXT_H_
#define FPDFSDK_FPDFXFA_CPDFXFA_CONTEXT_H_

#include <memory>
#include <vector>

#include "core/fxcrt/cfx_unowned_ptr.h"
#include "core/fxcrt/fx_system.h"
#include "core/fxcrt/observable.h"
#include "fpdfsdk/fpdfxfa/cpdfxfa_docenvironment.h"
#include "fpdfsdk/fpdfxfa/cpdfxfa_page.h"
#include "xfa/fxfa/cxfa_ffdoc.h"

class CJS_Runtime;
class CPDFSDK_FormFillEnvironment;
class CXFA_FFDocHandler;
class IJS_EventContext;
class IJS_Runtime;

enum LoadStatus {
  FXFA_LOADSTATUS_PRELOAD = 0,
  FXFA_LOADSTATUS_LOADING,
  FXFA_LOADSTATUS_LOADED,
  FXFA_LOADSTATUS_CLOSING,
  FXFA_LOADSTATUS_CLOSED
};

class CPDFXFA_Context : public IXFA_AppProvider {
 public:
  explicit CPDFXFA_Context(std::unique_ptr<CPDF_Document> pPDFDoc);
  ~CPDFXFA_Context() override;

  bool LoadXFADoc();
  CPDF_Document* GetPDFDoc() { return m_pPDFDoc.get(); }
  CXFA_FFDoc* GetXFADoc() { return m_pXFADoc.get(); }
  CXFA_FFDocView* GetXFADocView() { return m_pXFADocView.Get(); }
  XFA_DocType GetDocType() const { return m_iDocType; }
  v8::Isolate* GetJSERuntime() const;
  CXFA_FFApp* GetXFAApp() { return m_pXFAApp.get(); }

  CPDFSDK_FormFillEnvironment* GetFormFillEnv() const {
    return m_pFormFillEnv.Get();
  }
  void SetFormFillEnv(CPDFSDK_FormFillEnvironment* pFormFillEnv);

  int GetPageCount() const;
  void DeletePage(int page_index);
  RetainPtr<CPDFXFA_Page> GetXFAPage(int page_index);
  RetainPtr<CPDFXFA_Page> GetXFAPage(CXFA_FFPageView* pPage) const;
  void ClearChangeMark();

  // IFXA_AppProvider:
  WideString GetLanguage() override;
  WideString GetPlatform() override;
  WideString GetAppName() override;
  WideString GetAppTitle() const override;

  void Beep(uint32_t dwType) override;
  int32_t MsgBox(const WideString& wsMessage,
                 const WideString& wsTitle,
                 uint32_t dwIconType,
                 uint32_t dwButtonType) override;
  WideString Response(const WideString& wsQuestion,
                      const WideString& wsTitle,
                      const WideString& wsDefaultAnswer,
                      bool bMark) override;
  RetainPtr<IFX_SeekableReadStream> DownloadURL(
      const WideString& wsURL) override;
  bool PostRequestURL(const WideString& wsURL,
                      const WideString& wsData,
                      const WideString& wsContentType,
                      const WideString& wsEncode,
                      const WideString& wsHeader,
                      WideString& wsResponse) override;
  bool PutRequestURL(const WideString& wsURL,
                     const WideString& wsData,
                     const WideString& wsEncode) override;

  IFWL_AdapterTimerMgr* GetTimerMgr() override;

 protected:
  friend class CPDFXFA_DocEnvironment;

  int GetOriginalPageCount() const { return m_nPageCount; }
  void SetOriginalPageCount(int count) {
    m_nPageCount = count;
    m_XFAPageList.resize(count);
  }

  LoadStatus GetLoadStatus() const { return m_nLoadStatus; }
  std::vector<RetainPtr<CPDFXFA_Page>>* GetXFAPageList() {
    return &m_XFAPageList;
  }

 private:
  void CloseXFADoc();

  XFA_DocType m_iDocType;
  std::unique_ptr<CPDF_Document> m_pPDFDoc;
  std::unique_ptr<CXFA_FFDoc> m_pXFADoc;
  Observable<CPDFSDK_FormFillEnvironment>::ObservedPtr m_pFormFillEnv;
  CFX_UnownedPtr<CXFA_FFDocView> m_pXFADocView;
  std::unique_ptr<CXFA_FFApp> m_pXFAApp;
  std::unique_ptr<CJS_Runtime> m_pRuntime;
  std::vector<RetainPtr<CPDFXFA_Page>> m_XFAPageList;
  LoadStatus m_nLoadStatus;
  int m_nPageCount;

  // Must be destroyed before |m_pFormFillEnv|.
  CPDFXFA_DocEnvironment m_DocEnv;
};

#endif  // FPDFSDK_FPDFXFA_CPDFXFA_CONTEXT_H_
