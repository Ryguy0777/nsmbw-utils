.text
.extern DVD_GetFile__FPvPCcPCc
.extern DVDClass

.extern continueMultiCourseButtonSetup # 0x8079E178
.extern loadInfo__28dMultiCourseSelectContents_cFv
.global ReadDataFromLevelInfo
ReadDataFromLevelInfo:
    bl loadInfo__28dMultiCourseSelectContents_cFv
    b continueMultiCourseButtonSetup

.extern returnFromTextureCheck # 0x8079E044
.global CheckIfPreviewExists
CheckIfPreviewExists:
    bl DVD_GetFile__FPvPCcPCc # insn we replaced

    cmpwi r3, 0x0
    beq textureDoesNotExist
    b returnFromTextureCheck
textureDoesNotExist:
    lis r3, DVDClass@h
	ori r3, r3, DVDClass@l
    lwz r3, 0(r3)
	addi r3, r3, 4

    lis r4, ArcName@h
	ori r4, r4, ArcName@l

    lis r5, FallbackTextureName@h
	ori r5, r5, FallbackTextureName@l

    bl DVD_GetFile__FPvPCcPCc
    b returnFromTextureCheck

.data
ArcName: .string "MultiCorseSelectTexture"
.align 4
FallbackTextureName: .string "MultiCorseSelectTexture/im_W1_1.tpl"
.align 4