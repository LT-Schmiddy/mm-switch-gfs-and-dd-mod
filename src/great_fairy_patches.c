#include "modding.h"
#include "global.h"
#include "recomputils.h"
#include "recompconfig.h"


#include "overlays/actors/ovl_Bg_Dy_Yoseizo/z_bg_dy_yoseizo.h"
#include "overlays/actors/ovl_Demo_Effect/z_demo_effect.h"

#define FLAGS (ACTOR_FLAG_UPDATE_CULLING_DISABLED | ACTOR_FLAG_DRAW_CULLING_DISABLED | ACTOR_FLAG_UPDATE_DURING_OCARINA)

void BgDyYoseizo_Init(Actor* thisx, PlayState* play);
void BgDyYoseizo_Destroy(Actor* thisx, PlayState* play);
void BgDyYoseizo_Update(Actor* thisx, PlayState* play);
void BgDyYoseizo_Draw(Actor* thisx, PlayState* play);

void func_80A0B184(BgDyYoseizo* this, PlayState* play);
void func_80A0BB08(BgDyYoseizo* this, PlayState* play);

/* Effects functions */
void BgDyYoseizo_SpawnEffect(BgDyYoseizo* this, Vec3f* initPos, Vec3f* initVelocity, Vec3f* accel,
                             Color_RGB8* primColor, Color_RGB8* envColor, f32 scale, s16 life, s16 type);
void BgDyYoseizo_UpdateEffects(BgDyYoseizo* this, PlayState* play);
void BgDyYoseizo_DrawEffects(BgDyYoseizo* this, PlayState* play);
void func_80A0B290(BgDyYoseizo* this, PlayState* play);

void func_80A0B35C(BgDyYoseizo* this, PlayState* play) {
    BgDyYoseizo_Bob(this, play);
    SkelAnime_Update(&this->skelAnime);

    if (this->timer == 60) {
        if (!Flags_GetSwitch(play, GREAT_FAIRY_GET_SWITCHFLAG(&this->actor))) {
            switch (GREAT_FAIRY_GET_TYPE(&this->actor)) {
                case GREAT_FAIRY_TYPE_MAGIC:
                    if (gSaveContext.save.saveInfo.playerData.isMagicAcquired != true) {
                        gSaveContext.save.saveInfo.playerData.isMagicAcquired = true;
                        gSaveContext.magicFillTarget = MAGIC_NORMAL_METER;
                    }
                    break;

                case GREAT_FAIRY_TYPE_WISDOM:
                    if (gSaveContext.save.saveInfo.playerData.isDoubleMagicAcquired != true) {
                        gSaveContext.save.saveInfo.playerData.isDoubleMagicAcquired = true;
                        gSaveContext.magicFillTarget = MAGIC_DOUBLE_METER;
                        gSaveContext.save.saveInfo.playerData.magicLevel = 0;
                    }
                    break;

                case GREAT_FAIRY_TYPE_COURAGE:
                    if (gSaveContext.save.saveInfo.playerData.doubleDefense != true) {
                        gSaveContext.save.saveInfo.playerData.doubleDefense = true;
                    }
                    break;
                case GREAT_FAIRY_TYPE_POWER:
                    Item_Give(play, ITEM_SWORD_GREAT_FAIRY);
                    break;
                
                default:
                    break;
            }
        }
        Interface_SetHudVisibility(9);
    }

    if ((this->timer < 50) && (GREAT_FAIRY_GET_TYPE(&this->actor) == GREAT_FAIRY_TYPE_COURAGE)) {
        if (gSaveContext.save.saveInfo.inventory.defenseHearts < 20) {
            gSaveContext.save.saveInfo.inventory.defenseHearts++;
        }
    }

    if (this->timer == 50) {
        gSaveContext.healthAccumulator = 0x140;
        Magic_Add(play, MAGIC_FILL_TO_CAPACITY);
    }

    if (this->timer == 0) {
        this->beam->trigger = true;
        this->actionFunc = func_80A0B290;
    }
}