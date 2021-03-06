// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-License-Identifier: MIT

#include "azure/storage/common/account_sas_builder.hpp"
#include "azure/core/http/http.hpp"
#include "azure/storage/common/crypt.hpp"

namespace Azure { namespace Storage {

  void AccountSasBuilder::SetPermissions(AccountSasPermissions permissions)
  {
    Permissions.clear();
    if ((permissions & AccountSasPermissions::Read) == AccountSasPermissions::Read)
    {
      Permissions += "r";
    }
    if ((permissions & AccountSasPermissions::Write) == AccountSasPermissions::Write)
    {
      Permissions += "w";
    }
    if ((permissions & AccountSasPermissions::Delete) == AccountSasPermissions::Delete)
    {
      Permissions += "d";
    }
    if ((permissions & AccountSasPermissions::DeleteVersion)
        == AccountSasPermissions::DeleteVersion)
    {
      Permissions += "x";
    }
    if ((permissions & AccountSasPermissions::List) == AccountSasPermissions::List)
    {
      Permissions += "l";
    }
    if ((permissions & AccountSasPermissions::Add) == AccountSasPermissions::Add)
    {
      Permissions += "a";
    }
    if ((permissions & AccountSasPermissions::Create) == AccountSasPermissions::Create)
    {
      Permissions += "c";
    }
    if ((permissions & AccountSasPermissions::Update) == AccountSasPermissions::Update)
    {
      Permissions += "u";
    }
    if ((permissions & AccountSasPermissions::Process) == AccountSasPermissions::Process)
    {
      Permissions += "p";
    }
    if ((permissions & AccountSasPermissions::Tags) == AccountSasPermissions::Tags)
    {
      Permissions += "t";
    }
    if ((permissions & AccountSasPermissions::Filter) == AccountSasPermissions::Filter)
    {
      Permissions += "f";
    }
  }

  std::string AccountSasBuilder::GenerateSasToken(const StorageSharedKeyCredential& credential)
  {
    std::string protocol = Details::SasProtocolToString(Protocol);

    std::string services;
    if ((Services & AccountSasServices::Blobs) == AccountSasServices::Blobs)
    {
      services += "b";
    }
    if ((Services & AccountSasServices::Queue) == AccountSasServices::Queue)
    {
      services += "q";
    }
    if ((Services & AccountSasServices::Files) == AccountSasServices::Files)
    {
      services += "f";
    }

    std::string resourceTypes;
    if ((ResourceTypes & AccountSasResource::Service) == AccountSasResource::Service)
    {
      resourceTypes += "s";
    }
    if ((ResourceTypes & AccountSasResource::BlobContainer) == AccountSasResource::BlobContainer)
    {
      resourceTypes += "c";
    }
    if ((ResourceTypes & AccountSasResource::Object) == AccountSasResource::Object)
    {
      resourceTypes += "o";
    }

    std::string stringToSign = credential.AccountName + "\n" + Permissions + "\n" + services + "\n"
        + resourceTypes + "\n" + (StartsOn.HasValue() ? StartsOn.GetValue() : "") + "\n" + ExpiresOn
        + "\n" + (IPRange.HasValue() ? IPRange.GetValue() : "") + "\n" + protocol + "\n"
        + Details::DefaultSasVersion + "\n";

    std::string signature
        = Base64Encode(Details::HmacSha256(stringToSign, Base64Decode(credential.GetAccountKey())));

    Azure::Core::Http::Url builder;
    builder.AppendQueryParameter(
        "sv", Details::UrlEncodeQueryParameter(Details::DefaultSasVersion));
    builder.AppendQueryParameter("ss", Details::UrlEncodeQueryParameter(services));
    builder.AppendQueryParameter("srt", Details::UrlEncodeQueryParameter(resourceTypes));
    builder.AppendQueryParameter("sp", Details::UrlEncodeQueryParameter(Permissions));
    if (StartsOn.HasValue())
    {
      builder.AppendQueryParameter("st", Details::UrlEncodeQueryParameter(StartsOn.GetValue()));
    }
    builder.AppendQueryParameter("se", Details::UrlEncodeQueryParameter(ExpiresOn));
    if (IPRange.HasValue())
    {
      builder.AppendQueryParameter("sip", Details::UrlEncodeQueryParameter(IPRange.GetValue()));
    }
    builder.AppendQueryParameter("spr", Details::UrlEncodeQueryParameter(protocol));
    builder.AppendQueryParameter("sig", Details::UrlEncodeQueryParameter(signature));

    return builder.GetAbsoluteUrl();
  }

}} // namespace Azure::Storage
