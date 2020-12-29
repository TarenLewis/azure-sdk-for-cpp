
// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

namespace Azure { namespace Storage { namespace Files { namespace Shares {

  constexpr static const char* FileDefaultTimeValue = "now";
  constexpr static const char* FileCopySourceTime = "source";
  constexpr static const char* FileInheritPermission = "inherit";
  constexpr static const char* FilePreserveSmbProperties = "preserve";
  constexpr static const char* FileAllHandles = "*";

  namespace Details {
    constexpr int64_t FileUploadDefaultChunkSize = 4 * 1024 * 1024;
    constexpr int64_t FileDownloadDefaultChunkSize = 4 * 1024 * 1024;
    constexpr static const char* ShareSnapshotQueryParameter = "sharesnapshot";
  } // namespace Details

}}}} // namespace Azure::Storage::Files::Shares
