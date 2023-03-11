#pragma once

enum LOADINGID { LOADING_STAGE, LOADING_BOSS, LOADING_END };
enum WINMODE { MODE_FULL, MODE_WIN };

enum OBJ_TYPE {  OBJ_PLAYER, OBJ_MONSTER, OBJ_ARROW, OBJ_BULLET , OBJ_LANDSCAPE, OBJ_END };

enum ClAWDIR { CLAW_LEFT, CLAW_RIGHT }; // �� ȥ�� �� �

namespace Engine
{
	// ID_DYNAMIC : �� �����Ӹ��� ������ �ؾ��ϴ� ������Ʈ ����
	enum COMPONENTID{ID_DYNAMIC, ID_STATIC, ID_END };

	enum INFO {INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };
	enum ROTATION { ROT_X, ROT_Y, ROT_Z, ROT_END };
	enum TEXTYPE { TEX_NORMAL, TEX_CUBE, TEX_END };
	enum RENDERID { RENDER_PRIORITY, RENDER_ALPHA, RENDER_NONALPHA, RENDER_ALPHATEST, RENDER_UI, RENDER_END };

	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };
	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

	enum MONSTER_STATE { MONSTER_IDLE, MONSTER_MOVE, MONSTER_ATTACK, MONSTER_DEAD, MONSTER_END };
	enum MONSTER_HEAD { HEAD_FRONT, HEAD_BACK, HEAD_END };
	enum MONSTER_NAME { NAME_SLIME, NAME_SOUL, NAME_MUTATION, SOUL_BULLET, NAME_END };

	enum BULLET_STATE { BULLET_IDLE, BULLET_MOVE, BULLET_END };

	enum UI_CATEGORY { UI_NONE, UI_MENU, UI_FRONT, UI_END };

	enum BOW_TYPE { BOW_TYPE_SYLPH, BOW_TYPE_END };

	enum ARROW_TYPE { ARROW_TYPE_SYLPH, ARROW_TYPE_CHARGE_SYLPH, ARROW_TYPE_END };

	enum FOOT_TYPE { FOOT_TYPE_WATER, FOOT_TYPE_SMOKE, FOOT_TYPE_END };
}

enum LANDSCAPE_TYPE { STATIC_OBJ, DYNAMIC_OBJ };
enum STATIC_OBJECT_ID { MAIN_LOBBY, BOSS_DOWOLE_OBJ, STATIC_OBJ_END };
enum DYNAMIC_OBJECT_ID {
	BUSH1, BUSH2, BUSH3, BUSH4, BUSH5,
	BUSH_FLOWER1, BUSH_FLOWER2,
	THORN1, THORN2, THORN3, THORN4, THORN5, THORN6,
	THORN_DEST_EFFECT, THORN_HIT_EFFECT,
	DYNAMIC_OBJ_END
};