cmake_minimum_required(VERSION 3.20)
project(RagnarokOnlineDecomp LANGUAGES C CXX)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "Use an out-of-source build directory.")
endif()

add_subdirectory(ThirdParty)

add_library(RagnarokUtility STATIC
  Source/Utility/XorBytes.cpp
  Source/Utility/Base64.cpp
  Source/Utility/GroupedInteger.cpp
  Source/Utility/LanguageCharset.cpp
  Source/Utility/ClientAngles.cpp
  Source/Utility/AffineMatrix.cpp
  Source/Utility/AffineLookAt.cpp
  Source/Utility/RayTriangle.cpp
  Source/Utility/Rijndael.cpp
)

target_include_directories(RagnarokUtility PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/Source"
)
target_compile_features(RagnarokUtility PUBLIC cxx_std_20)
set_target_properties(RagnarokUtility PROPERTIES CXX_EXTENSIONS OFF)

if(WIN32)
  target_sources(RagnarokUtility PRIVATE
    Source/Utility/GuidParse.cpp
    Source/Utility/NSyncObject.cpp
    Source/Utility/CSyncObject.cpp
    Source/Utility/NThread.cpp
    Source/Utility/NMessageQueue.cpp
    Source/Utility/NMessageMgr.cpp
    Source/Utility/NObject.cpp
    Source/Utility/NRecycleObject.cpp
    Source/Utility/NRecycleObjectPoolMgr.cpp
    Source/Utility/NAsyncWork.cpp
    Source/Utility/NAsyncWorkQueue.cpp
    Source/Utility/NAsyncWorkThread.cpp
    Source/Utility/NAsyncWorkMgr.cpp
    Source/Utility/AsyncDiagnostic.cpp
    Source/Contents/InstantVar.cpp
    Source/Utility/FormatString.cpp
  )
  target_compile_definitions(RagnarokUtility PUBLIC
    WIN32_LEAN_AND_MEAN NOMINMAX _ITERATOR_DEBUG_LEVEL=0
  )
  add_library(RagnarokArchive STATIC
    Source/Archive/PakFile.cpp
    Source/Archive/PakName.cpp
    Source/Archive/PakCipher.cpp
    Source/Archive/GPak.cpp
    Source/Archive/MemMapPakFile.cpp
    Source/Archive/PakRegistry.cpp
  )
  target_include_directories(RagnarokArchive PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/Source"
  )
  target_compile_features(RagnarokArchive PUBLIC cxx_std_20)
  target_compile_definitions(RagnarokArchive PUBLIC
    WIN32_LEAN_AND_MEAN
    NOMINMAX
    _ITERATOR_DEBUG_LEVEL=0
    _CRT_SECURE_NO_WARNINGS
  )
  target_link_libraries(RagnarokArchive PUBLIC RagnarokUtility RagnarokZlib)
  set_target_properties(RagnarokArchive PROPERTIES CXX_EXTENSIONS OFF)

  add_library(RagnarokClientStartup STATIC
    Source/Client/Launcher.cpp
    Source/Client/ApplicationWindow.cpp
    Source/Client/ErrorDialog.cpp
    Source/Client/UnhandledExceptionHandler.cpp
    Source/Client/ModeCursor.cpp
    Source/Client/Mouse.cpp
    Source/Client/ClientOptions.cpp
    Source/Client/TextInputBuffer.cpp
    Source/Client/ImeInput.cpp
    Source/Client/TraditionalKeyInput.cpp
    Source/Client/InputCharset.cpp
    Source/Client/BrowserWindowSizing.cpp
    Source/Client/BrowserInterface.cpp
    Source/Client/BrowserCursor.cpp
    Source/Client/BrowserAddressEdit.cpp
    Source/Client/BrowserHostWindow.cpp
    Source/Client/BrowserCommandTarget.cpp
    Source/Client/BrowserInPlace.cpp
    Source/Client/BrowserDocumentUI.cpp
    Source/Client/BrowserClientSite.cpp
    Source/Client/BrowserAddressBar.cpp
    Source/Client/BrowserFrame.cpp
    Source/Client/BrowserSiteAttachment.cpp
    Source/Client/BrowserClose.cpp
    Source/Client/BrowserCreation.cpp
  )
  target_include_directories(RagnarokClientStartup PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/Source"
  )
  target_compile_features(RagnarokClientStartup PUBLIC cxx_std_20)
  target_compile_definitions(RagnarokClientStartup PUBLIC WIN32_LEAN_AND_MEAN NOMINMAX)
  target_link_libraries(RagnarokClientStartup PUBLIC RagnarokUtility user32 winmm imm32 ole32 oleaut32 uuid)
  set_target_properties(RagnarokClientStartup PROPERTIES CXX_EXTENSIONS OFF)

  add_library(RagnarokRender STATIC
    Source/Render/RenderPrimitive.cpp
    Source/Render/RenderPrimitivePool.cpp
    Source/Render/RenderCommandPool.cpp
    Source/Render/LightmappedPrimitive.cpp
    Source/Render/DepthAlpha.cpp
    Source/Render/RenderVertexPool.cpp
    Source/Render/RenderProjection.cpp
    Source/Render/RendererViewport.cpp
    Source/Render/RenderRay.cpp
    Source/Render/BitmapPixels.cpp
    Source/Render/Surface.cpp
    Source/Render/RenderQueues.cpp
    Source/Render/Texture.cpp
    Source/Render/UIRPData.cpp
    Source/Render/SurfaceDX9.cpp
    Source/Render/SurfaceDX7.cpp
    Source/Render/D3DX9Runtime.cpp
    Source/Render/TextureDX9.cpp
    Source/Render/TextureDX7.cpp
    Source/Render/TextureBindingDX7.cpp
    Source/Render/TextureAllocationDX7.cpp
    Source/Render/BitmapTextureCache.cpp
    Source/Render/SpriteAtlas.cpp
    Source/Render/DeviceContext.cpp
    Source/Render/DCBitmap.cpp
    Source/Render/DCSurface.cpp
    Source/Render/DeviceStateDX9.cpp
    Source/Render/DeviceOwnerDX9.cpp
    Source/Render/DeviceOwnerDX7.cpp
    Source/Render/DeviceCapabilitiesDX7.cpp
    Source/Render/TextureCalibrationDX7.cpp
    Source/Render/RenderAdapter.cpp
    Source/Render/RenderAdapterDX7.cpp
    Source/Render/RenderBackends.cpp
    Source/Render/DisplayMode.cpp
    Source/Render/DisplayModeDX7.cpp
    Source/Render/DirectDrawEnumeration.cpp
    Source/Render/DeviceRecoveryDX9.cpp
    Source/Render/BackBufferDX9.cpp
    Source/Render/BackBufferDX7.cpp
    Source/Render/Direct3DErrors.cpp
    Source/Render/RendererBackBuffer.cpp
    Source/Render/Renderer.cpp
    Source/Render/RendererDX9.cpp
    Source/Render/RenderSystemDX7.cpp
    Source/Render/RenderSystemDX9.cpp
    Source/Render/RendererDX9Operations.cpp
    Source/Render/RendererDX7Operations.cpp
    Source/Render/RenderToTextureDX7.cpp
    Source/Render/TextureModesDX9.cpp
    Source/Render/TextureModesDX7.cpp
    Source/Render/TextureStageModes.cpp
    Source/Render/RendererFogDX9.cpp
    Source/Render/RendererFogDX7.cpp
    Source/Render/OpaqueQueuesDX9.cpp
    Source/Render/GroundQueuesDX9.cpp
    Source/Render/AlphaQueuesDX9.cpp
    Source/Render/QueuedDrawingDX9.cpp
    Source/Render/RendererFrameDX9.cpp
    Source/Render/ScreenshotDX9.cpp
    Source/Render/ScreenshotDX7.cpp
    Source/Contents/Effect.cpp
    Source/Contents/EffectLists.cpp
    Source/Render/BlendColor.cpp
    Source/Render/RendererPresentation.cpp
    Source/Render/RendererPresentationDX7.cpp
    Source/Render/FontCache.cpp
    Source/Render/TextContext.cpp
  )
  target_include_directories(RagnarokRender PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Source")
  target_compile_features(RagnarokRender PUBLIC cxx_std_20)
  target_compile_definitions(RagnarokRender PUBLIC _ITERATOR_DEBUG_LEVEL=0 WIN32_LEAN_AND_MEAN NOMINMAX)
  set_target_properties(RagnarokRender PROPERTIES CXX_EXTENSIONS OFF)
  target_link_libraries(RagnarokRender PUBLIC gdi32 winmm gdiplus d3d9 ddraw dxguid)

  add_library(RagnarokUI STATIC
    Source/UI/WindowGeometry.cpp
    Source/UI/WindowCoordinates.cpp
    Source/UI/WindowAncestry.cpp
    Source/UI/WindowRegistration.cpp
    Source/UI/WindowRetirement.cpp
    Source/UI/WindowOrdering.cpp
    Source/UI/WindowMouseHit.cpp
    Source/UI/UIWindow.cpp
    Source/UI/UIControl.cpp
    Source/UI/UIDropDownBox.cpp
    Source/UI/ChatSuggestions.cpp
    Source/UI/ItemTagRetirement.cpp
    Source/UI/SnapRectangle.cpp
    Source/UI/SnapObject.cpp
    Source/UI/SnapMgr.cpp
    Source/UI/WindowSnapping.cpp
    Source/UI/FrameTextCache.cpp
    Source/UI/FrameFocus.cpp
    Source/UI/InputFocus.cpp
    Source/UI/UIFrameWnd.cpp
    Source/UI/ShortcutGrid.cpp
    Source/UI/WindowPolicy.cpp
    Source/UI/UIButton.cpp
    Source/UI/UIBasicButton.cpp
    Source/UI/UITextButton.cpp
    Source/UI/UINaviLinkButton.cpp
    Source/UI/UIEditCtrl.cpp
    Source/UI/UIEditVariants.cpp
    Source/UI/UIEditLinks.cpp
    Source/UI/UIBitmapButton.cpp
    Source/UI/UIBmpTextButton.cpp
    Source/UI/UIJobSlotButton.cpp
    Source/UI/UIPartyInfoWnd.cpp
    Source/UI/UIAdvenPartyBoardWnd.cpp
    Source/UI/UIApplyForPartyWnd.cpp
    Source/UI/UIRegisterPartyWnd.cpp
    Source/UI/UIRequestJoinPartyMgr.cpp
    Source/UI/UIRequestJoinPartyWnd.cpp
    Source/UI/UIChooseSellBuyWnd.cpp
    Source/UI/UIExchangeAcceptWnd.cpp
    Source/UI/UIJoinPartyAcceptWnd.cpp
    Source/Network/PartyJoinRequest.cpp
    Source/UI/UICheckBox.cpp
    Source/UI/UIScrollBar.cpp
    Source/UI/UIScrollBarNavi.cpp
    Source/UI/UIListBox.cpp
    Source/UI/UIListBoxEx.cpp
    Source/UI/UIComboBoxEx.cpp
    Source/UI/UIComboBoxDefWnd.cpp
    Source/UI/TextMarkup.cpp
    Source/UI/PixelTextWrapping.cpp
    Source/UI/ItemLinkDisplay.cpp
    Source/UI/UITextViewer.cpp
    Source/UI/UITextViewerLinks.cpp
    Source/UI/UITextViewerMessages.cpp
    Source/UI/UIStaticBitmap.cpp
    Source/UI/UIStaticText.cpp
    Source/UI/UIMessageBox.cpp
    Source/UI/WindowDialogs.cpp
    Source/Client/ReplayPacketRecorder.cpp
    Source/UI/CooldownState.cpp
    Source/UI/CooldownDrawing.cpp
    Source/UI/CooldownOwner.cpp
    Source/UI/ShortcutCooldowns.cpp
    Source/UI/ItemDrawing.cpp
    Source/UI/ShortcutPainting.cpp
    Source/UI/ShortcutBindings.cpp
    Source/UI/ShortcutLayout.cpp
    Source/UI/ShortcutActivation.cpp
    Source/UI/ShortcutControls.cpp
    Source/UI/ShortcutControlEvents.cpp
    Source/UI/ShortcutTooltip.cpp
    Source/UI/ShortcutPress.cpp
    Source/UI/ShortcutDrop.cpp
    Source/UI/ShortcutUse.cpp
    Source/UI/ShortcutVisibility.cpp
    Source/UI/ShortcutDetails.cpp
    Source/UI/SkillDetailClose.cpp
    Source/UI/NormalItemSlotLoad.cpp
    Source/UI/UIShortCutWnd.cpp
    Source/UI/TextWrapping.cpp
    Source/UI/ChatNotifications.cpp
    Source/UI/BrowserFactory.cpp
    Source/UI/BrowserPopupDispatch.cpp
    Source/UI/WindowFrameUpdate.cpp
    Source/UI/UIBalloonText.cpp
    Source/UI/UITransBalloonText.cpp
    Source/UI/ColoredText.cpp
    Source/UI/WindowBorder.cpp
    Source/UI/EditTextState.cpp
    Source/UI/EditViewport.cpp
    Source/UI/EditPainting.cpp
    Source/UI/PriceColor.cpp
    Source/UI/MerchantShopRefresh.cpp
    Source/UI/NavigationLink.cpp
    Source/UI/TextFitting.cpp
    Source/Client/ModeDragState.cpp
    Source/Client/ModeTooltip.cpp
    Source/Contents/CharacterConfigLoad.cpp
    Source/Contents/CharacterLinkedUserDataLoadAsyncWork.cpp
    Source/Contents/EmblemCache.cpp
    Source/Contents/EmblemUpload.cpp
    Source/Contents/ChangePartyMasterCompletion.cpp
    Source/Contents/PartyListCompletion.cpp
    Source/Contents/StopRecruitingCompletion.cpp
    Source/Contents/MyPartyCompletion.cpp
    Source/Contents/PartyRegistrationCompletion.cpp
    Source/Contents/PartyInfoCompletion.cpp
    Source/Contents/PartyJoinResult.cpp
    Source/Contents/PartyInvitation.cpp
  )
  target_include_directories(RagnarokUI PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Source")
  target_compile_features(RagnarokUI PUBLIC cxx_std_20)
  target_link_libraries(RagnarokUI PUBLIC RagnarokRender RagnarokClientStartup RagnarokResource RagnarokScript RagnarokContents)
  set_target_properties(RagnarokUI PROPERTIES CXX_EXTENSIONS OFF)

  add_library(RagnarokFile STATIC
    Source/File/ResourcePath.cpp
    Source/File/ShortcutFilename.cpp
    Source/File/MapName.cpp
    Source/File/File.cpp
    Source/File/CommonObject.cpp
  )
  target_include_directories(RagnarokFile PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Source")
  target_compile_features(RagnarokFile PUBLIC cxx_std_20)
  target_link_libraries(RagnarokFile PUBLIC RagnarokArchive RagnarokClientStartup)
  set_target_properties(RagnarokFile PROPERTIES CXX_EXTENSIONS OFF)

  add_library(RagnarokResource STATIC
    Source/Resource/ResourceAliases.cpp
    Source/Resource/BitmapImage.cpp
    Source/Resource/BitmapImagePng.cpp
    Source/Resource/BitmapImageJpeg.cpp
    Source/Resource/IJL15Runtime.cpp
    Source/Resource/MilesRuntime.cpp
    Source/Resource/WaveResource.cpp
    Source/Resource/Resource.cpp
    Source/Resource/BitmapResource.cpp
    Source/Resource/SpriteResource.cpp
    Source/Resource/SpriteEncoding.cpp
    Source/Resource/ActionResource.cpp
    Source/Resource/TerrainResource.cpp
    Source/Resource/GroundResource.cpp
    Source/Resource/WorldResource.cpp
    Source/Resource/WorldEnvironment.cpp
    Source/Resource/WorldObjects.cpp
    Source/Resource/ModelMesh.cpp
    Source/Resource/ModelResource.cpp
    Source/Resource/ModelNodeLoading.cpp
    Source/Resource/ModelResourceV2.cpp
    Source/Resource/ModelNodeLoadingV2.cpp
    Source/Resource/AnimatedModelResource.cpp
    Source/Contents/ModelAnimation.cpp
    Source/Contents/ModelQuaternion.cpp
    Source/Contents/ModelNode.cpp
    Source/Contents/ModelTextureAnimation.cpp
    Source/Contents/ModelRendering.cpp
    Source/Contents/ModelTextureUV.cpp
    Source/Contents/ModelLighting.cpp
    Source/Contents/ModelNodeBounds.cpp
    Source/Contents/ModelActor.cpp
    Source/Contents/ModelActorBounds.cpp
    Source/Contents/ModelPlayback.cpp
    Source/Contents/ModelOcclusion.cpp
    Source/Contents/ModelMovies.cpp
    Source/Contents/WorldModelPlacement.cpp
    Source/Contents/WorldModelLoading.cpp
    Source/Contents/WorldGroundImport.cpp
    Source/Contents/WorldEnvironment.cpp
    Source/Contents/WorldSpatialTree.cpp
    Source/Contents/WorldSpatialInitialization.cpp
    Source/Contents/WorldRayQuery.cpp
    Source/Contents/WorldOcclusion.cpp
    Source/Contents/WorldModelView.cpp
    Source/Contents/WorldViewSpatial.cpp
    Source/Contents/CameraSmoothing.cpp
    Source/Contents/CameraShake.cpp
    Source/Contents/CameraFrame.cpp
    Source/Contents/CameraQueries.cpp
    Source/Contents/ModelSpatial.cpp
    Source/Contents/GroundSpatial.cpp
    Source/Contents/GroundGeometry.cpp
    Source/Contents/GroundLightmaps.cpp
    Source/Contents/GroundRendering.cpp
    Source/Contents/GroundPicking.cpp
    Source/Contents/GroundDecals.cpp
    Source/Contents/GroundWaterState.cpp
    Source/Contents/GroundWaterRendering.cpp
    Source/Contents/GroundLighting.cpp
    Source/Contents/GroundSamples.cpp
    Source/Contents/GroundLightingInitialization.cpp
    Source/Render/TextureCache.cpp
    Source/Resource/EZeffectResource.cpp
    Source/Resource/ResourceMgr.cpp
    Source/Resource/ResourceFailures.cpp
    Source/Resource/ResourceTransient.cpp
    Source/Resource/TextureNames.cpp
    Source/Contents/SkinInfo.cpp
    Source/Resource/BinkRuntime.cpp
    Source/Contents/BinkMovie.cpp
    Source/Contents/BinkMovies.cpp
    Source/Render/ClientTextureSources.cpp
  )
  target_include_directories(RagnarokResource PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Source")
  target_compile_features(RagnarokResource PUBLIC cxx_std_20)
  target_compile_definitions(RagnarokResource PUBLIC _ITERATOR_DEBUG_LEVEL=0)
  target_link_libraries(RagnarokResource PUBLIC RagnarokPng RagnarokArchive RagnarokClientStartup RagnarokFile kernel32 winmm)
  set_target_properties(RagnarokResource PROPERTIES CXX_EXTENSIONS OFF)

  add_library(RagnarokNetwork STATIC
    Source/Network/CURLMgr.cpp
    Source/Network/CURLPostHander.cpp
    Source/Network/TextEncoding.cpp
    Source/Network/ConnectionBuffer.cpp
    Source/Network/Connection.cpp
    Source/Network/PacketLengths.cpp
    Source/Network/ConnectionRuntime.cpp
    Source/Network/ShortcutPacket.cpp
    Source/Network/PartyJoinPacket.cpp
  )
  target_include_directories(RagnarokNetwork PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Source")
  target_compile_features(RagnarokNetwork PUBLIC cxx_std_20)
  target_link_libraries(RagnarokNetwork PUBLIC RagnarokUtility RagnarokClientStartup oleaut32 ws2_32 winmm)
  set_target_properties(RagnarokNetwork PROPERTIES CXX_EXTENSIONS OFF)

  add_library(RagnarokContents STATIC
    Source/Contents/WhisperBlockMgr.cpp
    Source/Contents/CashEmotionMgr.cpp
    Source/Contents/AccountUserData.cpp
    Source/Contents/EmotionHotkeys.cpp
    Source/Contents/UserHotkeyScript.cpp
    Source/Contents/UserHotkeyMgr.cpp
    Source/Contents/AccountLinkedUserDataMgr.cpp
    Source/Contents/AccountService.cpp
    Source/Contents/ScriptOptions.cpp
    Source/Contents/GraphicsConfiguration.cpp
    Source/Contents/SoundConfiguration.cpp
    Source/Contents/SoundMgr.cpp
    Source/Contents/OptionWriter.cpp
    Source/Contents/MessageTable.cpp
    Source/Contents/AccountLinkedUserDataSaveAsyncWork.cpp
    Source/Contents/AccountLinkedUserDataLoadAsyncWork.cpp
    Source/Contents/CharacterLinkedUserDataMgr.cpp
    Source/Contents/CharacterLinkedUserDataSaveAsyncWork.cpp
    Source/Contents/CharacterConfigSave.cpp
    Source/Contents/SkillLevelConfig.cpp
    Source/Contents/SkillLevelRegistry.cpp
    Source/Contents/ServiceOptions.cpp
    Source/Contents/NormalItemSlots.cpp
    Source/Contents/SkillInfo.cpp
    Source/Contents/PlayerSkillBundle.cpp
    Source/Contents/SkillTreeScript.cpp
    Source/Contents/ClientSkills.cpp
    Source/Network/SkillUpdatePacket.cpp
    Source/Contents/ItemInfo.cpp
    Source/Contents/ItemDescription.cpp
    Source/Contents/ItemLink.cpp
    Source/Contents/TipBoxTitles.cpp
    Source/Contents/AutomaticSkills.cpp
    Source/Contents/AttackMotion.cpp
    Source/Client/ApplicationActivation.cpp
    Source/Client/ApplicationMessages.cpp
    Source/Client/BrowserInputHandoff.cpp
    Source/Client/WebBrowser.cpp
    Source/Client/BrowserRegistry.cpp
    Source/Client/WebEventReceiver.cpp
    Source/Client/BrowserMessages.cpp
    Source/Contents/HotkeyInfo.cpp
    Source/Contents/PlayerNameDictionary.cpp
    Source/Contents/ItemNaming.cpp
    Source/Contents/ItemResources.cpp
    Source/Contents/StoreBackupData.cpp
    Source/Contents/MCStoreInfoBackup.cpp
    Source/Contents/MCStoreInfoBackupAsyncWork.cpp
    Source/Contents/EmblemDownload.cpp
    Source/Contents/EmblemDataDownloadAsyncWork.cpp
    Source/Contents/EmblemDataMgr.cpp
    Source/Contents/GuildEmblemReqBundle.cpp
    Source/Contents/EmblemDataUploadAsyncWork.cpp
    Source/Contents/AdvenPartyInfo.cpp
    Source/Contents/AdventurerAgencyMgr.cpp
    Source/Contents/ChangePartyMasterAsyncWork.cpp
    Source/Contents/LoadPartyListAsyncWork.cpp
    Source/Contents/StopRecruitingAsyncWork.cpp
    Source/Contents/LoadMyPartyAsyncWork.cpp
    Source/Contents/RegisterPartyAsyncWork.cpp
    Source/Contents/LoadPartyInfoAsyncWork.cpp
    Source/Contents/AsyncWorkRecycleObjectPoolMgr.cpp
    Source/Contents/AsyncWorkMgr.cpp
    Source/Contents/AgencyRequests.cpp
    Source/Contents/ForbiddenWords.cpp
    Source/Contents/JobNames.cpp
    Source/Contents/SpriteDrawingPolicy.cpp
    Source/Contents/JobInheritance.cpp
    Source/Contents/MessengerMember.cpp
    Source/Contents/ClientMessenger.cpp
    Source/Contents/RenderObjectState.cpp
    Source/Contents/GameObject.cpp
    Source/Contents/RenderObject.cpp
    Source/Contents/WorldObjects.cpp
    Source/Contents/WorldActors.cpp
    Source/Contents/EffectAttachments.cpp
    Source/Contents/EffectTypes.cpp
    Source/Contents/RagEffectState.cpp
    Source/Contents/RagEffectInitialization.cpp
    Source/Contents/RagEffectResourceSelection.cpp
    Source/Contents/EffectPrimitiveState.cpp
    Source/Contents/EffectPrimitiveColor.cpp
    Source/Contents/EffectPrimitiveMotion.cpp
    Source/Contents/EffectPrimitiveRendering.cpp
    Source/Contents/EffectPrimitiveRecords.cpp
    Source/Contents/EffectParticle.cpp
    Source/Contents/PartyEffect.cpp
    Source/Contents/EffectActor.cpp
    Source/Contents/AttachedEffect.cpp
    Source/Contents/PartyMemberEffects.cpp
    Source/Contents/EffectVisibility.cpp
    Source/Contents/TerrainHeight.cpp
    Source/Contents/TerrainCoordinates.cpp
    Source/Contents/TerrainPicking.cpp
    Source/Contents/TerrainPlane.cpp
    Source/Contents/WorldTerrainPicking.cpp
    Source/Contents/WorldProximity.cpp
    Source/Contents/JobNameInitialization.cpp
    Source/Contents/ShowRankingScheduler.cpp
  )
  target_include_directories(RagnarokContents PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Source")
  target_compile_features(RagnarokContents PUBLIC cxx_std_20)
  target_link_libraries(RagnarokContents PUBLIC RagnarokNetwork RagnarokJson RagnarokScript RagnarokFile RagnarokResource winmm advapi32)
  set_target_properties(RagnarokContents PROPERTIES CXX_EXTENSIONS OFF)

  add_library(RagnarokScript STATIC
    Source/Script/LuaScript.cpp
    Source/Script/LuaInstance.cpp
  )
  target_include_directories(RagnarokScript PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Source")
  target_compile_features(RagnarokScript PUBLIC cxx_std_20)
  target_link_libraries(RagnarokScript PUBLIC RagnarokLua RagnarokUtility user32)
  set_target_properties(RagnarokScript PROPERTIES CXX_EXTENSIONS OFF)
endif()

enable_testing()
