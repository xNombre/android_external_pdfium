// Copyright 2016 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef CORE_FPDFAPI_RENDER_CPDF_DOCRENDERDATA_H_
#define CORE_FPDFAPI_RENDER_CPDF_DOCRENDERDATA_H_

#include <map>

#include "core/fpdfapi/page/cpdf_countedobject.h"
#include "core/fpdfapi/render/cpdf_transferfunc.h"
#include "core/fxcrt/cfx_unowned_ptr.h"
#include "core/fxcrt/fx_system.h"

class CPDF_Document;
class CPDF_Font;
class CPDF_Object;
class CPDF_Type3Cache;
class CPDF_Type3Font;

class CPDF_DocRenderData {
 public:
  explicit CPDF_DocRenderData(CPDF_Document* pPDFDoc);
  ~CPDF_DocRenderData();

  RetainPtr<CPDF_Type3Cache> GetCachedType3(CPDF_Type3Font* pFont);
  void MaybePurgeCachedType3(CPDF_Type3Font* pFont);

  RetainPtr<CPDF_TransferFunc> GetTransferFunc(CPDF_Object* pObj);
  void MaybePurgeTransferFunc(CPDF_Object* pOb);

  void Clear(bool bRelease);

 private:
  CFX_UnownedPtr<CPDF_Document> m_pPDFDoc;
  std::map<CPDF_Font*, RetainPtr<CPDF_Type3Cache>> m_Type3FaceMap;
  std::map<CPDF_Object*, RetainPtr<CPDF_TransferFunc>> m_TransferFuncMap;
};

#endif  // CORE_FPDFAPI_RENDER_CPDF_DOCRENDERDATA_H_
