cmake_minimum_required(VERSION 3.20)
project(RagnarokOnlineDecomp LANGUAGES C CXX)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "Use an out-of-source build directory.")
endif()

add_subdirectory(ThirdParty)

add_library(RagnarokUtility STATIC
  Source/Utility/Cipher/XorBytes.cpp
  Source/Utility/Text/Base64.cpp
  Source/Utility/Text/GroupedInteger.cpp
  Source/Utility/Text/LanguageCharset.cpp
  Source/Utility/Math/ClientAngles.cpp
  Source/Utility/Math/AffineMatrix.cpp
  Source/Utility/Math/AffineLookAt.cpp
  Source/Utility/Math/RayTriangle.cpp
  Source/Utility/Cipher/Rijndael.cpp
)

target_include_directories(RagnarokUtility PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/Source"
)
target_compile_features(RagnarokUtility PUBLIC cxx_std_20)
set_target_properties(RagnarokUtility PROPERTIES CXX_EXTENSIONS OFF)

if(WIN32)
  target_sources(RagnarokUtility PRIVATE
    Source/Utility/GuidParse.cpp
    Source/Utility/Threading/NSyncObject.cpp
    Source/Utility/Threading/NThread.cpp
    Source/Utility/Async/NMessageQueue.cpp
    Source/Utility/Async/NMessageMgr.cpp
    Source/Utility/Async/NObject.cpp
    Source/Utility/Async/NRecycleObjectPoolMgr.cpp
    Source/Utility/Async/NAsyncWork.cpp
    Source/Utility/Async/NAsyncWorkQueue.cpp
    Source/Utility/Async/NAsyncWorkThread.cpp
    Source/Utility/Async/NAsyncWorkMgr.cpp
    Source/Utility/Async/AsyncDiagnostic.cpp
    Source/Contents/InstantVar.cpp
    Source/Utility/Text/FormatString.cpp
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
    Source/Client/Application/Launcher.cpp
    Source/Client/Application/ApplicationWindow.cpp
    Source/Client/Application/ErrorDialog.cpp
    Source/Client/Application/UnhandledExceptionHandler.cpp
    Source/Client/Mode/ModeCursor.cpp
    Source/Client/Mode/ActiveGameMode.cpp
    Source/Client/Input/Mouse.cpp
    Source/Client/ClientOptions.cpp
    Source/Client/Input/TextInputBuffer.cpp
    Source/Client/Input/ImeInput.cpp
    Source/Client/Input/TraditionalKeyInput.cpp
    Source/Client/Input/InputCharset.cpp
    Source/Client/Browser/BrowserWindowSizing.cpp
    Source/Client/Browser/BrowserInterface.cpp
    Source/Client/Browser/BrowserCursor.cpp
    Source/Client/Browser/BrowserAddressEdit.cpp
    Source/Client/Browser/BrowserHostWindow.cpp
    Source/Client/Browser/BrowserCommandTarget.cpp
    Source/Client/Browser/BrowserInPlace.cpp
    Source/Client/Browser/BrowserDocumentUI.cpp
    Source/Client/Browser/BrowserClientSite.cpp
    Source/Client/Browser/BrowserAddressBar.cpp
    Source/Client/Browser/BrowserFrame.cpp
    Source/Client/Browser/BrowserSiteAttachment.cpp
    Source/Client/Browser/BrowserClose.cpp
    Source/Client/Browser/BrowserCreation.cpp
  )
  target_include_directories(RagnarokClientStartup PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/Source"
  )
  target_compile_features(RagnarokClientStartup PUBLIC cxx_std_20)
  target_compile_definitions(RagnarokClientStartup PUBLIC WIN32_LEAN_AND_MEAN NOMINMAX)
  target_link_libraries(RagnarokClientStartup PUBLIC RagnarokUtility user32 winmm imm32 ole32 oleaut32 uuid)
  set_target_properties(RagnarokClientStartup PROPERTIES CXX_EXTENSIONS OFF)

  add_library(RagnarokRender STATIC
    Source/Render/Primitives/RenderPrimitive.cpp
    Source/Render/Primitives/RenderPrimitivePool.cpp
    Source/Render/Primitives/RenderCommandPool.cpp
    Source/Render/Primitives/LightmappedPrimitive.cpp
    Source/Render/Primitives/DepthAlpha.cpp
    Source/Render/Primitives/RenderVertexPool.cpp
    Source/Render/Projection/RenderProjection.cpp
    Source/Render/Projection/RendererViewport.cpp
    Source/Render/Projection/RenderRay.cpp
    Source/Render/Textures/BitmapPixels.cpp
    Source/Render/Textures/Surface.cpp
    Source/Render/Primitives/RenderQueues.cpp
    Source/Render/Textures/Texture.cpp
    Source/Render/Primitives/UIRPData.cpp
    Source/Render/DX9/SurfaceDX9.cpp
    Source/Render/DX7/SurfaceDX7.cpp
    Source/Render/DX9/TextureDX9.cpp
    Source/Render/DX7/TextureDX7.cpp
    Source/Render/DX7/TextureBindingDX7.cpp
    Source/Render/Textures/BitmapTextureCache.cpp
    Source/Render/Textures/SpriteAtlas.cpp
    Source/Render/DeviceContext/DeviceContext.cpp
    Source/Render/DeviceContext/DCBitmap.cpp
    Source/Render/DeviceContext/DCSurface.cpp
    Source/Render/DX9/DeviceStateDX9.cpp
    Source/Render/DX9/DeviceOwnerDX9.cpp
    Source/Render/DX7/DeviceOwnerDX7.cpp
    Source/Render/DX7/DeviceCapabilitiesDX7.cpp
    Source/Render/DX7/TextureCalibrationDX7.cpp
    Source/Render/RenderAdapter.cpp
    Source/Render/DX7/RenderAdapterDX7.cpp
    Source/Render/RenderBackends.cpp
    Source/Render/DisplayMode.cpp
    Source/Render/DX7/DisplayModeDX7.cpp
    Source/Render/DX7/DirectDrawEnumeration.cpp
    Source/Render/DX9/DeviceRecoveryDX9.cpp
    Source/Render/DX9/BackBufferDX9.cpp
    Source/Render/DX7/BackBufferDX7.cpp
    Source/Render/Direct3DErrors.cpp
    Source/Render/DX9/RendererBackBuffer.cpp
    Source/Render/Renderer.cpp
    Source/Render/DX9/RendererDX9.cpp
    Source/Render/DX7/RenderSystemDX7.cpp
    Source/Render/DX9/RenderSystemDX9.cpp
    Source/Render/DX9/RendererDX9Operations.cpp
    Source/Render/DX7/RendererDX7Operations.cpp
    Source/Render/DX7/RenderToTextureDX7.cpp
    Source/Render/DX9/TextureModesDX9.cpp
    Source/Render/DX7/TextureModesDX7.cpp
    Source/Render/DX9/RendererFogDX9.cpp
    Source/Render/DX7/RendererFogDX7.cpp
    Source/Render/DX9/OpaqueQueuesDX9.cpp
    Source/Render/DX9/GroundQueuesDX9.cpp
    Source/Render/DX9/AlphaQueuesDX9.cpp
    Source/Render/DX9/QueuedDrawingDX9.cpp
    Source/Render/DX9/RendererFrameDX9.cpp
    Source/Render/DX9/ScreenshotDX9.cpp
    Source/Render/DX7/ScreenshotDX7.cpp
    Source/Contents/Effects/Effect.cpp
    Source/Contents/Effects/EffectLists.cpp
    Source/Render/BlendColor.cpp
    Source/Render/DX9/RendererPresentation.cpp
    Source/Render/DX7/RendererPresentationDX7.cpp
    Source/Render/Text/FontCache.cpp
    Source/Render/Text/TextContext.cpp
  )
  target_include_directories(RagnarokRender PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Source")
  target_compile_features(RagnarokRender PUBLIC cxx_std_20)
  target_compile_definitions(RagnarokRender PUBLIC _ITERATOR_DEBUG_LEVEL=0 WIN32_LEAN_AND_MEAN NOMINMAX)
  set_target_properties(RagnarokRender PROPERTIES CXX_EXTENSIONS OFF)
  target_link_libraries(RagnarokRender PUBLIC gdi32 winmm gdiplus d3d9 ddraw dxguid)

  add_library(RagnarokUI STATIC
    Source/UI/Layout/WindowCoordinates.cpp
    Source/UI/Window/WindowAncestry.cpp
    Source/UI/Window/WindowRegistration.cpp
    Source/UI/Window/WindowRetirement.cpp
    Source/UI/Window/WindowOrdering.cpp
    Source/UI/Window/WindowMouseHit.cpp
    Source/UI/Window/UIWindow.cpp
    Source/UI/Window/UIControl.cpp
    Source/UI/Lists/UIDropDownBox.cpp
    Source/UI/Chat/ChatSuggestions.cpp
    Source/UI/Edit/ItemTagRetirement.cpp
    Source/UI/Layout/SnapRectangle.cpp
    Source/UI/Layout/SnapObject.cpp
    Source/UI/Layout/SnapMgr.cpp
    Source/UI/Layout/WindowSnapping.cpp
    Source/UI/Window/FrameTextCache.cpp
    Source/UI/Window/FrameFocus.cpp
    Source/UI/Window/InputFocus.cpp
    Source/UI/Window/UIFrameWnd.cpp
    Source/UI/Shortcuts/ShortcutGrid.cpp
    Source/UI/Window/WindowPolicy.cpp
    Source/UI/Buttons/UIButton.cpp
    Source/UI/Buttons/UIBasicButton.cpp
    Source/UI/Buttons/UITextButton.cpp
    Source/UI/Buttons/UINaviLinkButton.cpp
    Source/UI/Edit/UIEditCtrl.cpp
    Source/UI/Edit/UIEditVariants.cpp
    Source/UI/Edit/UIEditLinks.cpp
    Source/UI/Buttons/UIBitmapButton.cpp
    Source/UI/Buttons/UIBmpTextButton.cpp
    Source/UI/Buttons/UIJobSlotButton.cpp
    Source/UI/Party/UIPartyInfoWnd.cpp
    Source/UI/Party/UIAdvenPartyBoardWnd.cpp
    Source/UI/Party/UIApplyForPartyWnd.cpp
    Source/UI/Party/UIRegisterPartyWnd.cpp
    Source/UI/Party/UIRequestJoinPartyMgr.cpp
    Source/UI/Party/UIRequestJoinPartyWnd.cpp
    Source/UI/Acceptance/UIChooseSellBuyWnd.cpp
    Source/UI/Acceptance/UIExchangeAcceptWnd.cpp
    Source/UI/Acceptance/UIJoinPartyAcceptWnd.cpp
    Source/Network/Packets/PartyJoinRequest.cpp
    Source/UI/Buttons/UICheckBox.cpp
    Source/UI/Lists/UIScrollBar.cpp
    Source/UI/Lists/UIScrollBarNavi.cpp
    Source/UI/Lists/UIListBox.cpp
    Source/UI/Lists/UIListBoxEx.cpp
    Source/UI/Lists/UIComboBoxEx.cpp
    Source/UI/Lists/UIComboBoxDefWnd.cpp
    Source/UI/Text/TextMarkup.cpp
    Source/UI/Text/PixelTextWrapping.cpp
    Source/UI/Text/ItemLinkDisplay.cpp
    Source/UI/Text/UITextViewer.cpp
    Source/UI/Text/UITextViewerLinks.cpp
    Source/UI/Text/UITextViewerMessages.cpp
    Source/UI/UIStaticBitmap.cpp
    Source/UI/Text/UIStaticText.cpp
    Source/UI/Window/UIMessageBox.cpp
    Source/UI/Window/WindowDialogs.cpp
    Source/Client/ReplayPacketRecorder.cpp
    Source/UI/Cooldowns/CooldownState.cpp
    Source/UI/Cooldowns/CooldownDrawing.cpp
    Source/UI/Cooldowns/CooldownOwner.cpp
    Source/UI/Shortcuts/ShortcutCooldowns.cpp
    Source/UI/Shop/ItemDrawing.cpp
    Source/UI/Shortcuts/ShortcutPainting.cpp
    Source/UI/Shortcuts/ShortcutBindings.cpp
    Source/UI/Shortcuts/ShortcutActivation.cpp
    Source/UI/Shortcuts/ShortcutControls.cpp
    Source/UI/Shortcuts/ShortcutTooltip.cpp
    Source/UI/Shortcuts/ShortcutPress.cpp
    Source/UI/Shortcuts/ShortcutDrop.cpp
    Source/UI/Shortcuts/ShortcutDetails.cpp
    Source/UI/Shortcuts/NormalItemSlotLoad.cpp
    Source/UI/Shortcuts/UIShortCutWnd.cpp
    Source/UI/Text/TextWrapping.cpp
    Source/Client/Browser/BrowserFactory.cpp
    Source/Client/Browser/BrowserPopupDispatch.cpp
    Source/UI/Window/WindowFrameUpdate.cpp
    Source/UI/Text/UIBalloonText.cpp
    Source/UI/Text/UITransBalloonText.cpp
    Source/UI/Text/ColoredText.cpp
    Source/UI/Window/WindowBorder.cpp
    Source/UI/Edit/EditTextState.cpp
    Source/UI/Edit/EditViewport.cpp
    Source/UI/Shop/PriceColor.cpp
    Source/UI/Shop/MerchantShopRefresh.cpp
    Source/UI/Text/NavigationLink.cpp
    Source/UI/Text/TextFitting.cpp
    Source/Client/Mode/ModeDragState.cpp
    Source/Client/Mode/ModeTooltip.cpp
    Source/Contents/UserData/CharacterConfigLoad.cpp
    Source/Contents/UserData/CharacterLinkedUserDataLoadAsyncWork.cpp
    Source/Contents/Emblems/EmblemCache.cpp
    Source/Contents/Emblems/EmblemUpload.cpp
    Source/Contents/Party/ChangePartyMasterCompletion.cpp
    Source/Contents/Party/PartyListCompletion.cpp
    Source/Contents/Party/StopRecruitingCompletion.cpp
    Source/Contents/Party/MyPartyCompletion.cpp
    Source/Contents/Party/PartyRegistrationCompletion.cpp
    Source/Contents/Party/PartyInfoCompletion.cpp
    Source/Contents/Party/PartyJoinResult.cpp
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
    Source/Resource/Bitmap/BitmapImage.cpp
    Source/Resource/Bitmap/BitmapImagePng.cpp
    Source/Resource/Bitmap/BitmapImageJpeg.cpp
    Source/Resource/Audio/WaveResource.cpp
    Source/Resource/Resource.cpp
    Source/Resource/Bitmap/BitmapResource.cpp
    Source/Resource/Sprites/SpriteResource.cpp
    Source/Resource/Sprites/SpriteEncoding.cpp
    Source/Resource/Sprites/ActionResource.cpp
    Source/Resource/World/TerrainResource.cpp
    Source/Resource/World/GroundResource.cpp
    Source/Resource/World/WorldResource.cpp
    Source/Resource/Models/ModelMesh.cpp
    Source/Resource/Models/ModelResource.cpp
    Source/Resource/Models/ModelResourceV2.cpp
    Source/Resource/Models/AnimatedModelResource.cpp
    Source/Contents/Models/ModelAnimation.cpp
    Source/Contents/Models/ModelQuaternion.cpp
    Source/Contents/Models/ModelNode.cpp
    Source/Contents/Models/ModelTextureAnimation.cpp
    Source/Contents/Models/ModelRendering.cpp
    Source/Contents/Models/ModelLighting.cpp
    Source/Contents/Models/ModelNodeBounds.cpp
    Source/Contents/Models/ModelActor.cpp
    Source/Contents/Models/ModelActorBounds.cpp
    Source/Contents/Models/ModelPlayback.cpp
    Source/Contents/Models/ModelOcclusion.cpp
    Source/Contents/Models/ModelMovies.cpp
    Source/Contents/World/WorldModelLoading.cpp
    Source/Contents/World/WorldSpatialTree.cpp
    Source/Contents/World/WorldSpatialInitialization.cpp
    Source/Contents/World/WorldRayQuery.cpp
    Source/Contents/World/WorldOcclusion.cpp
    Source/Contents/World/WorldModelView.cpp
    Source/Contents/World/WorldViewSpatial.cpp
    Source/Contents/Camera/CameraSmoothing.cpp
    Source/Contents/Camera/CameraShake.cpp
    Source/Contents/Camera/CameraFrame.cpp
    Source/Contents/Camera/CameraQueries.cpp
    Source/Contents/Models/ModelSpatial.cpp
    Source/Contents/Ground/GroundSpatial.cpp
    Source/Contents/Ground/GroundGeometry.cpp
    Source/Contents/Ground/GroundLightmaps.cpp
    Source/Contents/Ground/GroundRendering.cpp
    Source/Contents/Ground/GroundPicking.cpp
    Source/Contents/Ground/GroundDecals.cpp
    Source/Contents/Ground/GroundWaterState.cpp
    Source/Contents/Ground/GroundWaterRendering.cpp
    Source/Contents/Ground/GroundLighting.cpp
    Source/Contents/Ground/GroundSamples.cpp
    Source/Contents/Ground/GroundLightingInitialization.cpp
    Source/Render/Textures/TextureCache.cpp
    Source/Resource/EZeffectResource.cpp
    Source/Resource/ResourceMgr.cpp
    Source/Resource/ResourceFailures.cpp
    Source/Resource/ResourceTransient.cpp
    Source/Resource/TextureNames.cpp
    Source/Contents/SkinInfo.cpp
    Source/Contents/Movies/BinkMovie.cpp
    Source/Contents/Movies/BinkMovies.cpp
  )
  target_include_directories(RagnarokResource PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Source")
  target_compile_features(RagnarokResource PUBLIC cxx_std_20)
  target_compile_definitions(RagnarokResource PUBLIC _ITERATOR_DEBUG_LEVEL=0)
  target_link_libraries(RagnarokResource PUBLIC RagnarokPng RagnarokArchive RagnarokClientStartup RagnarokFile kernel32 winmm)
  set_target_properties(RagnarokResource PROPERTIES CXX_EXTENSIONS OFF)

  add_library(RagnarokNetwork STATIC
    Source/Network/Http/CURLMgr.cpp
    Source/Network/Http/CURLPostHander.cpp
    Source/Network/TextEncoding.cpp
    Source/Network/Connection/ConnectionBuffer.cpp
    Source/Network/Connection/Connection.cpp
    Source/Network/Packets/PacketLengths.cpp
    Source/Network/Connection/ConnectionRuntime.cpp
  )
  target_include_directories(RagnarokNetwork PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Source")
  target_compile_features(RagnarokNetwork PUBLIC cxx_std_20)
  target_link_libraries(RagnarokNetwork PUBLIC RagnarokUtility RagnarokClientStartup oleaut32 ws2_32 winmm)
  set_target_properties(RagnarokNetwork PROPERTIES CXX_EXTENSIONS OFF)

  add_library(RagnarokContents STATIC
    Source/Contents/Messenger/WhisperBlockMgr.cpp
    Source/Contents/CashEmotionMgr.cpp
    Source/Contents/UserData/AccountUserData.cpp
    Source/Contents/Hotkeys/EmotionHotkeys.cpp
    Source/Contents/Hotkeys/UserHotkeyScript.cpp
    Source/Contents/Hotkeys/UserHotkeyMgr.cpp
    Source/Contents/UserData/AccountLinkedUserDataMgr.cpp
    Source/Contents/UserData/AccountService.cpp
    Source/Contents/Configuration/ScriptOptions.cpp
    Source/Contents/Sound/SoundConfiguration.cpp
    Source/Contents/Sound/SoundMgr.cpp
    Source/Contents/Configuration/OptionWriter.cpp
    Source/Contents/MessageTable.cpp
    Source/Contents/UserData/AccountLinkedUserDataSaveAsyncWork.cpp
    Source/Contents/UserData/AccountLinkedUserDataLoadAsyncWork.cpp
    Source/Contents/UserData/CharacterLinkedUserDataMgr.cpp
    Source/Contents/UserData/CharacterLinkedUserDataSaveAsyncWork.cpp
    Source/Contents/UserData/CharacterConfigSave.cpp
    Source/Contents/Skills/SkillLevelConfig.cpp
    Source/Contents/Skills/SkillLevelRegistry.cpp
    Source/Contents/Configuration/ServiceOptions.cpp
    Source/Contents/UserData/NormalItemSlots.cpp
    Source/Contents/Skills/SkillInfo.cpp
    Source/Contents/Skills/PlayerSkillBundle.cpp
    Source/Contents/Skills/SkillTreeScript.cpp
    Source/Contents/Skills/ClientSkills.cpp
    Source/Network/Packets/SkillUpdatePacket.cpp
    Source/Contents/Items/ItemInfo.cpp
    Source/Contents/Items/ItemDescription.cpp
    Source/Contents/Items/ItemLink.cpp
    Source/Contents/TipBoxTitles.cpp
    Source/Contents/Skills/AutomaticSkills.cpp
    Source/Contents/Actors/AttackMotion.cpp
    Source/Client/Application/ApplicationActivation.cpp
    Source/Client/Application/ApplicationMessages.cpp
    Source/Client/Browser/BrowserInputHandoff.cpp
    Source/Client/Browser/WebBrowser.cpp
    Source/Client/Browser/BrowserRegistry.cpp
    Source/Client/Browser/WebEventReceiver.cpp
    Source/Client/Browser/BrowserMessages.cpp
    Source/Contents/Hotkeys/HotkeyInfo.cpp
    Source/Contents/PlayerNameDictionary.cpp
    Source/Contents/Items/ItemNaming.cpp
    Source/Contents/Items/ItemResources.cpp
    Source/Contents/Store/StoreBackupData.cpp
    Source/Contents/Store/MCStoreInfoBackup.cpp
    Source/Contents/Store/MCStoreInfoBackupAsyncWork.cpp
    Source/Contents/Emblems/EmblemDownload.cpp
    Source/Contents/Emblems/EmblemDataDownloadAsyncWork.cpp
    Source/Contents/Emblems/EmblemDataMgr.cpp
    Source/Contents/Emblems/GuildEmblemReqBundle.cpp
    Source/Contents/Emblems/EmblemDataUploadAsyncWork.cpp
    Source/Contents/Party/AdvenPartyInfo.cpp
    Source/Contents/Party/AdventurerAgencyMgr.cpp
    Source/Contents/Party/ChangePartyMasterAsyncWork.cpp
    Source/Contents/Party/LoadPartyListAsyncWork.cpp
    Source/Contents/Party/StopRecruitingAsyncWork.cpp
    Source/Contents/Party/LoadMyPartyAsyncWork.cpp
    Source/Contents/Party/RegisterPartyAsyncWork.cpp
    Source/Contents/Party/LoadPartyInfoAsyncWork.cpp
    Source/Contents/Async/AsyncWorkRecycleObjectPoolMgr.cpp
    Source/Contents/Async/AsyncWorkMgr.cpp
    Source/Contents/Party/AgencyRequests.cpp
    Source/Contents/ForbiddenWords.cpp
    Source/Contents/Jobs/JobNames.cpp
    Source/Contents/Actors/SpriteDrawingPolicy.cpp
    Source/Contents/Jobs/JobInheritance.cpp
    Source/Contents/Messenger/MessengerMember.cpp
    Source/Contents/Messenger/ClientMessenger.cpp
    Source/Contents/Actors/RenderObjectState.cpp
    Source/Contents/Actors/GameObject.cpp
    Source/Contents/Actors/RenderObject.cpp
    Source/Contents/World/WorldObjects.cpp
    Source/Contents/World/WorldActors.cpp
    Source/Contents/Effects/EffectAttachments.cpp
    Source/Contents/Effects/EffectTypes.cpp
    Source/Contents/Effects/RagEffectInitialization.cpp
    Source/Contents/Effects/EffectPrimitiveState.cpp
    Source/Contents/Effects/EffectPrimitiveColor.cpp
    Source/Contents/Effects/EffectPrimitiveMotion.cpp
    Source/Contents/Effects/EffectPrimitiveRendering.cpp
    Source/Contents/Effects/EffectPrimitiveRecords.cpp
    Source/Contents/Effects/EffectParticle.cpp
    Source/Contents/Effects/PartyEffect.cpp
    Source/Contents/Effects/EffectActor.cpp
    Source/Contents/Effects/AttachedEffect.cpp
    Source/Contents/Effects/PartyMemberEffects.cpp
    Source/Contents/Effects/EffectVisibility.cpp
    Source/Contents/Ground/TerrainHeight.cpp
    Source/Contents/Ground/TerrainCoordinates.cpp
    Source/Contents/Ground/TerrainPicking.cpp
    Source/Contents/Ground/TerrainPlane.cpp
    Source/Contents/World/WorldTerrainPicking.cpp
    Source/Contents/World/WorldProximity.cpp
    Source/Contents/Jobs/JobNameInitialization.cpp
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
