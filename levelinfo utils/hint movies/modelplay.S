.text
.extern returnFromModelPlayLI # 0x807945A8
.extern loadInfo__16dModelPlayDate_cFv

.global ReadLevelNumberFromLevelInfo
ReadLevelNumberFromLevelInfo:
    or r3, r30, r30
    bl loadInfo__16dModelPlayDate_cFv
    b returnFromModelPlayLI