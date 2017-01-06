#include "Maskter.h"
#define	FLOAT2FIX2(F)			((PF_Fixed)((F) * 65536 + (((F) < 0) ? -1 : 1)))
static PF_Err 
About (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	AEGP_SuiteHandler suites(in_data->pica_basicP);
	
	suites.ANSICallbacksSuite1()->sprintf(	out_data->return_msg,
											"Hello, thank you for using my plugin!!\n\nFor any bug reports, questions,\nor any features wanted,\nplease cantact me at\n  benzngf@gmail.com  \nThank you~~");
	return PF_Err_NONE;
}

static PF_Err 
GlobalSetup (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	out_data->my_version = PF_VERSION(	MAJOR_VERSION, 
										MINOR_VERSION,
										BUG_VERSION, 
										STAGE_VERSION, 
										BUILD_VERSION);
	out_data->out_flags = 0L;
	out_data->out_flags2 = PF_OutFlag2_PARAM_GROUP_START_COLLAPSED_FLAG;
	
	return PF_Err_NONE;
}

enum PARAMS{
	LAYERPARAM = 1,
	PATHPARAM,
	MODEPOPUP,
	NUMBER,
	DIST,
	SCALE,
	ROTATION,
	AUTORIENT,
	ALPHA,
	STARTP,
	ENDP,
	TRANSMODE,
	TIMEINTERCHK,
	GROUPST,
	SCALEATSTART,
	SCALEATEND,
	ROTATEATSTART,
	ROTATEATEND,
	ALPHAATSTART,
	ALPHAATEND,
	TIMEGROUP,
	TIMEMODE,
	TIMESHIFT,
	GROUPEND1,
	GROUPEND2,
	BACKG
};

static PF_Err 
ParamsSetup (	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err		err		= PF_Err_NONE;
	PF_ParamDef	def;	

	AEFX_CLR_STRUCT(def);

	def.param_type = PF_Param_LAYER;
	PF_STRCPY(def.name, "Choose emit layer");
	def.uu.id = 1;
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;
	if (err = PF_ADD_PARAM(in_data, -1, &def))
		return err;
	AEFX_CLR_STRUCT(def);

	def.param_type = PF_Param_PATH;
	PF_STRCPY(def.name, "Choose a path (mask)");
	def.uu.id = 2;
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;
	if (err = PF_ADD_PARAM(in_data, -1, &def))
		return err;
	AEFX_CLR_STRUCT(def);

	A_char PopupStr_1[100];
	PF_STRCPY(PopupStr_1, "Const. particle number|Const. dist. between");
	def.param_type = PF_Param_POPUP;
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;
	PF_STRCPY(def.name, "Mode¡G");
	def.flags |= PF_ParamFlag_SUPERVISE;
	def.uu.id = 3;
	def.u.pd.num_choices = 2;
	def.u.pd.dephault = 1;
	def.u.pd.value = def.u.pd.dephault;
	def.u.pd.u.namesptr = PopupStr_1;
	if (err = PF_ADD_PARAM(in_data, -1, &def))
		return err;
	AEFX_CLR_STRUCT(def);

	def.param_type = PF_Param_FLOAT_SLIDER;
	PF_STRCPY(def.name, "Num. of particles");
	def.uu.id = 9;
	def.u.fs_d.precision = 0;
	def.u.fs_d.dephault = 1;
	def.u.fs_d.slider_min = 1;
	def.u.fs_d.slider_max = 100;
	def.u.fs_d.valid_max = 1000;
	def.u.fs_d.valid_min = (PF_FpShort)0.1;
	def.u.fs_d.display_flags = 0;
	//if (params[MODEPOPUP]->u.pd.value == 2) def.ui_flags = PF_PUI_DISABLED;
	if (err = PF_ADD_PARAM(in_data, -1, &def))
		return err;
	AEFX_CLR_STRUCT(def);

	def.param_type = PF_Param_FLOAT_SLIDER;
	PF_STRCPY(def.name, "Dist. between");
	def.uu.id = 4;
	def.u.fs_d.precision = 1;
	def.u.fs_d.dephault = 100;
	def.u.fs_d.slider_min = 10;
	def.u.fs_d.slider_max = 100;
	def.u.fs_d.valid_max = 5000;
	def.u.fs_d.valid_min = 5;
	def.u.fs_d.display_flags = 0;
	//if (params[MODEPOPUP]->u.pd.value == 1) def.ui_flags = PF_PUI_DISABLED;
	if (err = PF_ADD_PARAM(in_data, -1, &def))
		return err;
	AEFX_CLR_STRUCT(def);

	def.param_type = PF_Param_FLOAT_SLIDER;
	PF_STRCPY(def.name, "Master scale");
	def.uu.id = 5;
	def.u.fs_d.precision = 2;
	def.u.fs_d.dephault = 1;
	def.u.fs_d.slider_min = 0;
	def.u.fs_d.slider_max = 1;
	def.u.fs_d.valid_max = 10;
	def.u.fs_d.valid_min = 0;
	def.u.fs_d.display_flags = 0;
	if (err = PF_ADD_PARAM(in_data, -1, &def))
		return err;
	AEFX_CLR_STRUCT(def);
	
	def.param_type = PF_Param_ANGLE;
	PF_STRCPY(def.name, "Master rotation");
	def.uu.id = 6;
	def.u.ad.dephault = 0;
	def.u.ad.valid_max = 360000 << 16;
	def.u.ad.valid_min = -360000 << 16;
	if (err = PF_ADD_PARAM(in_data, -1, &def))
		return err;
	AEFX_CLR_STRUCT(def);
	
	def.param_type = PF_Param_CHECKBOX;
	PF_STRCPY(def.name, "Orient along path");
	def.uu.id = 10;
	def.u.bd.dephault = false;
	A_char name[10];
	PF_STRCPY(name, "YEAH!");
	def.u.bd.u.nameptr = name;
	if (err = PF_ADD_PARAM(in_data, -1, &def))
		return err;
	AEFX_CLR_STRUCT(def);

	def.param_type = PF_Param_FLOAT_SLIDER;
	PF_STRCPY(def.name, "Alpha");
	def.uu.id = 19;
	def.u.fs_d.precision = 2;
	def.u.fs_d.dephault = 1;
	def.u.fs_d.slider_min = 0;
	def.u.fs_d.slider_max = 1;
	def.u.fs_d.valid_max = 1;
	def.u.fs_d.valid_min = 0;
	def.u.fs_d.display_flags = 0;
	if (err = PF_ADD_PARAM(in_data, -1, &def))
		return err;
	AEFX_CLR_STRUCT(def);

	def.param_type = PF_Param_FLOAT_SLIDER;
	PF_STRCPY(def.name, "Start point");
	def.uu.id = 7;
	def.u.fs_d.dephault = 0;
	def.u.fs_d.slider_min = 0;
	def.u.fs_d.slider_max = 100;
	def.u.fs_d.valid_max = 100;
	def.u.fs_d.valid_min = 0;
	def.u.fs_d.precision = 0;
	def.u.fs_d.display_flags = 1;
	if (err = PF_ADD_PARAM(in_data, -1, &def))
		return err;
	AEFX_CLR_STRUCT(def);

	def.param_type = PF_Param_FLOAT_SLIDER;
	PF_STRCPY(def.name, "End point");
	def.uu.id = 8;
	def.u.fs_d.dephault = 100;
	def.u.fs_d.slider_min = 0;
	def.u.fs_d.slider_max = 100;
	def.u.fs_d.valid_max = 100;
	def.u.fs_d.valid_min = 0;
	def.u.fs_d.precision = 0;
	def.u.fs_d.display_flags = 1;
	if (err = PF_ADD_PARAM(in_data, -1, &def))
		return err;
	AEFX_CLR_STRUCT(def);

	enum Popup_2{ NORMAL = 1, BEHIND, MULTIPLY, SCREEN };
	A_char PopupStr_2[100];
	PF_STRCPY(PopupStr_2, "Normal¡]End point on top¡^|Normal¡]Start point on top¡^|Multiply|Screen");
	def.param_type = PF_Param_POPUP;
	PF_STRCPY(def.name, "Blend mode¡G");
	def.uu.id = 20;
	def.u.pd.num_choices = 4;
	def.u.pd.dephault = NORMAL;
	def.u.pd.value = def.u.pd.dephault;
	def.u.pd.u.namesptr = PopupStr_2;
	if (err = PF_ADD_PARAM(in_data, -1, &def))
		return err;
	AEFX_CLR_STRUCT(def);

	def.param_type = PF_Param_CHECKBOX;
	PF_STRCPY(def.name, "Time displacement");
	def.uu.id = 22;
	def.flags = (PF_ParamFlag_SUPERVISE | PF_ParamFlag_CANNOT_TIME_VARY);
	def.u.bd.dephault = false;
	PF_STRCPY(name, "YEAH!!!!!!");
	def.u.bd.u.nameptr = name;
	if (err = PF_ADD_PARAM(in_data, -1, &def))
		return err;
	AEFX_CLR_STRUCT(def);

	def.param_type = PF_Param_GROUP_START;
	PF_STRCPY(def.name, "Start-end displacement");
	def.uu.id = 11;
	def.flags = PF_ParamFlag_START_COLLAPSED;
	if (err = PF_ADD_PARAM(in_data, -1, &def))
		return err;
	AEFX_CLR_STRUCT(def);

	def.param_type = PF_Param_FLOAT_SLIDER;
	PF_STRCPY(def.name, "Scale¡]At start point¡^");
	def.uu.id = 12;
	def.u.fs_d.precision = 2;
	def.u.fs_d.dephault = 1;
	def.u.fs_d.slider_min = 0;
	def.u.fs_d.slider_max = 1;
	def.u.fs_d.valid_max = 10;
	def.u.fs_d.valid_min = 0;
	def.u.fs_d.display_flags = 0;
	if (err = PF_ADD_PARAM(in_data, -1, &def))
		return err;
	AEFX_CLR_STRUCT(def);

	def.param_type = PF_Param_FLOAT_SLIDER;
	PF_STRCPY(def.name, "Scale¡]At end point¡^");
	def.uu.id = 13;
	def.u.fs_d.precision = 2;
	def.u.fs_d.dephault = 1;
	def.u.fs_d.slider_min = 0;
	def.u.fs_d.slider_max = 1;
	def.u.fs_d.valid_max = 10;
	def.u.fs_d.valid_min = 0;
	def.u.fs_d.display_flags = 0;
	if (err = PF_ADD_PARAM(in_data, -1, &def))
		return err;
	AEFX_CLR_STRUCT(def);

	def.param_type = PF_Param_ANGLE;
	PF_STRCPY(def.name, "Rotation¡]At start point¡^");
	def.uu.id = 14;
	def.u.ad.dephault = 0;
	def.u.ad.valid_max = 360000 << 16;
	def.u.ad.valid_min = -360000 << 16;
	if (err = PF_ADD_PARAM(in_data, -1, &def))
		return err;
	AEFX_CLR_STRUCT(def);

	def.param_type = PF_Param_ANGLE;
	PF_STRCPY(def.name, "Rotation¡]At end point¡^");
	def.uu.id = 15;
	def.u.ad.dephault = 0;
	def.u.ad.valid_max = 360000 << 16;
	def.u.ad.valid_min = -360000 << 16;
	if (err = PF_ADD_PARAM(in_data, -1, &def))
		return err;
	AEFX_CLR_STRUCT(def);

	def.param_type = PF_Param_FLOAT_SLIDER;
	PF_STRCPY(def.name, "Alpha¡]At start point¡^");
	def.uu.id = 16;
	def.u.fs_d.precision = 2;
	def.u.fs_d.dephault = 1;
	def.u.fs_d.slider_min = 0;
	def.u.fs_d.slider_max = 1;
	def.u.fs_d.valid_max = 1;
	def.u.fs_d.valid_min = 0;
	def.u.fs_d.display_flags = 0;
	if (err = PF_ADD_PARAM(in_data, -1, &def))
		return err;
	AEFX_CLR_STRUCT(def);

	def.param_type = PF_Param_FLOAT_SLIDER;
	PF_STRCPY(def.name, "Alpha¡]At end point¡^");
	def.uu.id = 17;
	def.u.fs_d.precision = 2;
	def.u.fs_d.dephault = 1;
	def.u.fs_d.slider_min = 0;
	def.u.fs_d.slider_max = 1;
	def.u.fs_d.valid_max = 1;
	def.u.fs_d.valid_min = 0;
	def.u.fs_d.display_flags = 0;
	if (err = PF_ADD_PARAM(in_data, -1, &def))
		return err;
	AEFX_CLR_STRUCT(def);

	def.param_type = PF_Param_GROUP_START;
	PF_STRCPY(def.name, "Time displacement");
	def.uu.id = 23;
	def.flags = PF_ParamFlag_START_COLLAPSED;
	//if (params[TIMEINTERCHK]->u.pd.value == false) def.ui_flags = PF_PUI_DISABLED;
	if (err = PF_ADD_PARAM(in_data, -1, &def))
		return err;
	AEFX_CLR_STRUCT(def);

	PF_STRCPY(PopupStr_2, "Freeze frame¡]From start¡^|Move along timeline");
	def.param_type = PF_Param_POPUP;
	PF_STRCPY(def.name, "Time mode¡G");
	def.uu.id = 25;
	def.u.pd.num_choices = 2;
	def.u.pd.dephault = 1;
	def.u.pd.value = def.u.pd.dephault;
	//if (params[TIMEINTERCHK]->u.pd.value == false) def.ui_flags = PF_PUI_DISABLED;
	def.u.pd.u.namesptr = PopupStr_2;
	if (err = PF_ADD_PARAM(in_data, -1, &def))
		return err;
	AEFX_CLR_STRUCT(def);

	def.param_type = PF_Param_FLOAT_SLIDER;
	PF_STRCPY(def.name, "End point time (from start)(frames)");
	def.uu.id = 26;
	def.u.fs_d.precision = 0;
	def.u.fs_d.dephault = 0;
	def.u.fs_d.slider_min = -100;
	def.u.fs_d.slider_max = 100;
	def.u.fs_d.valid_max = 1000;
	def.u.fs_d.valid_min = -1000;
	//if (params[TIMEINTERCHK]->u.pd.value == false) def.ui_flags = PF_PUI_DISABLED;
	def.u.fs_d.display_flags = 0;
	if (err = PF_ADD_PARAM(in_data, -1, &def))
		return err;
	AEFX_CLR_STRUCT(def);

	def.param_type = PF_Param_GROUP_END;
	def.uu.id = 24;
	if (err = PF_ADD_PARAM(in_data, -1, &def))
		return err;
	AEFX_CLR_STRUCT(def);

	def.param_type = PF_Param_GROUP_END;
	def.uu.id = 18;
	if (err = PF_ADD_PARAM(in_data, -1, &def))
		return err;
	AEFX_CLR_STRUCT(def);

	A_char PopupStr_3[30];
	PF_STRCPY(PopupStr_3, "Original layer|Transparent");
	def.param_type = PF_Param_POPUP;
	PF_STRCPY(def.name, "Background¡G");
	def.uu.id = 21;
	def.u.pd.num_choices = 2;
	def.u.pd.dephault = 1;
	def.u.pd.value = def.u.pd.dephault;
	def.u.pd.u.namesptr = PopupStr_3;
	if (err = PF_ADD_PARAM(in_data, -1, &def))
		return err;
	AEFX_CLR_STRUCT(def);

	

	out_data->num_params = 27;
	return err;
}




static PF_Err
ChangeParam(
PF_InData		*in_data,
PF_ParamDef		*params[],
PF_OutData  *out_data,
PF_LayerDef		*output,
PF_UserChangedParamExtra *extra){

	AEGP_SuiteHandler suites(in_data->pica_basicP);
	PF_ParamDef	def;
	AEFX_CLR_STRUCT(def);
	
	if (extra->param_index == MODEPOPUP){
		switch (params[MODEPOPUP]->u.pd.value){
		case 1:
			def.param_type = PF_Param_FLOAT_SLIDER;
			PF_STRCPY(def.name, "Dist. between");
			def.uu.id = 4;
			def.u.fs_d.precision = 1;
			def.u.fs_d.dephault = 100;
			def.u.fs_d.slider_min = 10;
			def.u.fs_d.slider_max = 100;
			def.u.fs_d.valid_max = 5000;
			def.u.fs_d.valid_min = 5;
			def.u.fs_d.display_flags = 0;
			def.flags = PF_ParamFlag_START_COLLAPSED;
			def.ui_flags = PF_PUI_DISABLED;
			suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref, DIST, &def);
			AEFX_CLR_STRUCT(def);

			def.param_type = PF_Param_FLOAT_SLIDER;
			PF_STRCPY(def.name, "Num. of particles");
			def.uu.id = 9;
			def.u.fs_d.precision = 0;
			def.u.fs_d.dephault = 1;
			def.u.fs_d.slider_min = 1;
			def.u.fs_d.slider_max = 100;
			def.u.fs_d.valid_max = 1000;
			def.u.fs_d.valid_min = (PF_FpShort)0.1;
			def.u.fs_d.display_flags = 0;
			def.flags = PF_ParamFlag_START_COLLAPSED;
			def.ui_flags = 0L;
			suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref, NUMBER, &def);
			AEFX_CLR_STRUCT(def);
			break;
		case 2:
			def.param_type = PF_Param_FLOAT_SLIDER;
			PF_STRCPY(def.name, "Dist. between");
			def.uu.id = 4;
			def.u.fs_d.precision = 1;
			def.u.fs_d.dephault = 100;
			def.u.fs_d.slider_min = 10;
			def.u.fs_d.slider_max = 100;
			def.u.fs_d.valid_max = 5000;
			def.u.fs_d.valid_min = 5;
			def.u.fs_d.display_flags = 0;
			def.flags = PF_ParamFlag_START_COLLAPSED;
			def.ui_flags = 0L;
			suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref, DIST, &def);
			AEFX_CLR_STRUCT(def);

			def.param_type = PF_Param_FLOAT_SLIDER;
			PF_STRCPY(def.name, "Num. of particles");
			def.uu.id = 9;
			def.u.fs_d.precision = 0;
			def.u.fs_d.dephault = 1;
			def.u.fs_d.slider_min = 1;
			def.u.fs_d.slider_max = 100;
			def.u.fs_d.valid_max = 1000;
			def.u.fs_d.valid_min = (PF_FpShort)0.1;
			def.u.fs_d.display_flags = 0;
			def.flags = PF_ParamFlag_START_COLLAPSED;
			def.ui_flags = PF_PUI_DISABLED;
			suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref, NUMBER, &def);
			AEFX_CLR_STRUCT(def);
			break;
		default:
			break;
		}
	}
	else if (extra->param_index == TIMEINTERCHK){
		if (params[TIMEINTERCHK]->u.bd.value){
			def.param_type = PF_Param_GROUP_START;
			PF_STRCPY(def.name, "Time displacement");
			def.uu.id = 23;
			def.flags = 0L;
			def.ui_flags = 0L;
			suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref, TIMEGROUP, &def);
			AEFX_CLR_STRUCT(def);

			A_char PopupStr_2[50];
			PF_STRCPY(PopupStr_2, "Freeze frame¡]From start¡^|Move along timeline");
			def.param_type = PF_Param_POPUP;
			PF_STRCPY(def.name, "Time mode¡G");
			def.uu.id = 25;
			def.u.pd.num_choices = 2;
			def.u.pd.dephault = 1;
			def.u.pd.value = def.u.pd.dephault;
			def.ui_flags = 0L;
			def.u.pd.u.namesptr = PopupStr_2;
			suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref, TIMEMODE, &def);
			AEFX_CLR_STRUCT(def);

			def.param_type = PF_Param_FLOAT_SLIDER;
			PF_STRCPY(def.name, "End point time (from start)(frames)");
			def.uu.id = 26;
			def.u.fs_d.precision = 0;
			def.u.fs_d.dephault = 0;
			def.u.fs_d.slider_min = -100;
			def.u.fs_d.slider_max = 100;
			def.u.fs_d.valid_max = 1000;
			def.u.fs_d.valid_min = -1000;
			def.flags = PF_ParamFlag_START_COLLAPSED;
			def.ui_flags = 0L;
			def.u.fs_d.display_flags = 0; 
			suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref, TIMESHIFT, &def);
			AEFX_CLR_STRUCT(def);
		}
		else{
			def.param_type = PF_Param_GROUP_START;
			PF_STRCPY(def.name, "Time displacement");
			def.uu.id = 23;
			def.flags = PF_ParamFlag_START_COLLAPSED;
			def.ui_flags = PF_PUI_DISABLED;
			suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref, TIMEGROUP, &def);
			AEFX_CLR_STRUCT(def);

			A_char PopupStr_2[50];
			PF_STRCPY(PopupStr_2, "Freeze frame¡]From start¡^|Move along timeline");
			def.param_type = PF_Param_POPUP;
			PF_STRCPY(def.name, "Time mode¡G");
			def.uu.id = 25;
			def.u.pd.num_choices = 2;
			def.u.pd.dephault = 1;
			def.u.pd.value = def.u.pd.dephault;
			def.ui_flags = PF_PUI_DISABLED;
			def.u.pd.u.namesptr = PopupStr_2;
			suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref, TIMEMODE, &def);
			AEFX_CLR_STRUCT(def);

			def.param_type = PF_Param_FLOAT_SLIDER;
			PF_STRCPY(def.name, "End point time (from start)(frames)");
			def.uu.id = 26;
			def.u.fs_d.precision = 0;
			def.u.fs_d.dephault = 0;
			def.u.fs_d.slider_min = -100;
			def.u.fs_d.slider_max = 100;
			def.u.fs_d.valid_max = 1000;
			def.u.fs_d.valid_min = -1000;
			def.flags = PF_ParamFlag_START_COLLAPSED;
			def.ui_flags = PF_PUI_DISABLED;
			def.u.fs_d.display_flags = 0;
			suites.ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref, TIMESHIFT, &def);
			AEFX_CLR_STRUCT(def);
		}
	}
	
	return PF_Err_NONE;
}


static PF_Err 
Render(
PF_InData		*in_data,
PF_OutData		*out_data,
PF_ParamDef		*params[],
PF_LayerDef		*output)
{
	PF_Err				err = PF_Err_NONE;
	AEGP_SuiteHandler	suites(in_data->pica_basicP);
	/*	Put interesting code here. */
	if ((params[TRANSMODE]->u.pd.value == 2L) || (params[BACKG]->u.pd.value == 2L)){
		suites.FillMatteSuite2()->fill(in_data->effect_ref, NULL, NULL, output);
	}else{
		suites.WorldTransformSuite1()->copy_hq(in_data->effect_ref, &params[0]->u.ld, output, NULL, &output->extent_hint);
	}

	PF_PathOutlinePtr ThePath;
	if (params[PATHPARAM]->u.path_d.path_id == NULL) return err;
	suites.PathQuerySuite1()->PF_CheckoutPath(in_data->effect_ref, params[PATHPARAM]->u.path_d.path_id, in_data->current_time, in_data->time_step, in_data->time_scale, &ThePath);
	A_long i,num_segments;
	//PF_FpLong j;
	PF_PathVertex Processvertex;
	PF_ParamDef checkout;
	PF_LayerDef checkedOutLayer;
	PF_Point Render_Origin;

	PF_FloatMatrix transFormMatrix;

	PF_CompositeMode CompMode;
	CompMode.opacity = (A_u_char)((PF_FpLong)255*params[ALPHA]->u.fs_d.value);
	CompMode.rand_seed = 0;
	CompMode.rgb_only = false;
	switch (params[TRANSMODE]->u.pd.value)
	{
	case 1L:
		CompMode.xfer = PF_Xfer_IN_FRONT;
		break;
	case 2L:
		CompMode.xfer = PF_Xfer_BEHIND;
		break;
	case 3L:
		CompMode.xfer = PF_Xfer_MULTIPLY;
		break;
	case 4L:
		CompMode.xfer = PF_Xfer_SCREEN;
		break;
	}
	A_Boolean NeedsChkOut = false;
	A_long CurrentChkoutTime = (params[TIMEMODE]->u.pd.value == 1) ? 0 : in_data->current_time;
	A_long TimeTmp = 0, TimeTmpPrev = 0;
	if (params[TIMEINTERCHK]->u.bd.value == false){
		err = PF_CHECKOUT_PARAM(in_data, 1, in_data->current_time,
			in_data->time_step, in_data->time_scale, &checkout);
			if (err) return err;
			if (!checkout.u.ld.data) {		// nothing to do
				suites.PathQuerySuite1()->PF_CheckinPath(in_data->effect_ref, params[PATHPARAM]->u.path_d.path_id, false, ThePath);
				return err;
			}
			checkedOutLayer = checkout.u.ld;
			NeedsChkOut = false;
	}
	else{
		err = PF_CHECKOUT_PARAM(in_data, 1, CurrentChkoutTime,
			in_data->time_step, in_data->time_scale, &checkout);
		if (err) return err;
		checkedOutLayer = checkout.u.ld;
		NeedsChkOut = true;
	}
	

	
	
	suites.PathDataSuite1()->PF_PathNumSegments(in_data->effect_ref, ThePath, &num_segments);
	

	PF_FpLong scale = params[SCALE]->u.fs_d.value;
	PF_FpLong angle = FIX_2_FLOAT(params[ROTATION]->u.ad.value);
	PF_FpLong OrientAngle;
	/*PF_EffectWorld AfterTransform;
	PF_NewWorldFlags Worldflags = 0;
	Worldflags |= PF_NewWorldFlag_CLEAR_PIXELS;
	if (PF_WORLD_IS_DEEP(&checkedOutLayer))
	Worldflags |= PF_NewWorldFlag_DEEP_PIXELS;*/

	//calculate path length
	PF_FpLong Sum_length = 0;
	PF_PathSegPrepPtr PrepPtr;
	typedef struct segmentL{
		PF_FpLong Value;
		struct segmentL *next;
	} SegmentL;
	SegmentL *segmentLength = (SegmentL*)malloc(sizeof(SegmentL));
	SegmentL *FirstSegL = segmentLength;
	for (i = 0; i < num_segments; i++){
		segmentLength->next = (SegmentL*)malloc(sizeof(SegmentL));
		suites.PathDataSuite1()->PF_PathPrepareSegLength(in_data->effect_ref, ThePath, i,50, &PrepPtr);
		suites.PathDataSuite1()->PF_PathGetSegLength(in_data->effect_ref, ThePath, i, &PrepPtr,&segmentLength->Value);
		suites.PathDataSuite1()->PF_PathCleanupSegLength(in_data->effect_ref, ThePath, i, &PrepPtr);
		Sum_length += segmentLength->Value;
		segmentLength = segmentLength->next;
	}
	segmentLength = FirstSegL;
	i = 0;
	PF_FpLong NowSegLength = Sum_length * params[STARTP]->u.fs_d.value / 100;
	PF_FpLong EndPoint = Sum_length * params[ENDP]->u.fs_d.value / 100;
	A_long Num, k;
	PF_FpLong DistBetween;
	PF_FpLong scaleTemp, AngleTemp;
	A_u_char OpacityTemp = CompMode.opacity;
	if (params[MODEPOPUP]->u.pd.value == 1){
		Num = FIX2INT(FLOAT2FIX2(params[NUMBER]->u.fs_d.value));
		DistBetween = (EndPoint - NowSegLength) / Num;
	}else{
		DistBetween = params[DIST]->u.fs_d.value;
		Num = (A_long)((EndPoint - NowSegLength)/DistBetween);
	}
	//RENDER START!!!!!!!!!!!!!

	

		if (DistBetween <= 0){
			k = Num;
		}else{
			k = 0;
		}
		if (Num == 1){
			while (NowSegLength >= segmentLength->Value){
				++i;
				NowSegLength -= segmentLength->Value;
				segmentLength = segmentLength->next;
			}
			suites.PathDataSuite1()->PF_PathPrepareSegLength(in_data->effect_ref, ThePath, i, 10, &PrepPtr);
			if (params[AUTORIENT]->u.bd.value){
				suites.PathDataSuite1()->PF_PathEvalSegLengthDeriv1(in_data->effect_ref, ThePath, &PrepPtr, i, NowSegLength,
					&Processvertex.x, &Processvertex.y, &Processvertex.tan_in_x, &Processvertex.tan_in_y);
				OrientAngle = (suites.ANSICallbacksSuite1()->atan2(Processvertex.tan_in_y, Processvertex.tan_in_x)) / PF_RAD_PER_DEGREE;
			}
			else{
				suites.PathDataSuite1()->PF_PathEvalSegLength(in_data->effect_ref, ThePath, &PrepPtr, i, NowSegLength, &Processvertex.x, &Processvertex.y);
				OrientAngle = 0;
			}
			suites.PathDataSuite1()->PF_PathCleanupSegLength(in_data->effect_ref, ThePath, i, &PrepPtr);
			CompMode.opacity = (A_u_char)((PF_FpLong)OpacityTemp *((params[ALPHAATSTART]->u.fs_d.value)*(1 - ((PF_FpLong)k / ((PF_FpLong)Num))) + (params[ALPHAATEND]->u.fs_d.value)*((PF_FpLong)k / ((PF_FpLong)Num))));
			scaleTemp = scale*((params[SCALEATSTART]->u.fs_d.value)*(1 - ((PF_FpLong)k / ((PF_FpLong)Num))) + (params[SCALEATEND]->u.fs_d.value)*((PF_FpLong)k / ((PF_FpLong)Num)));
			AngleTemp = FIX_2_FLOAT(params[ROTATEATSTART]->u.ad.value) * (1 - ((PF_FpLong)k / ((PF_FpLong)Num))) + FIX_2_FLOAT(params[ROTATEATEND]->u.ad.value) * ((PF_FpLong)k / ((PF_FpLong)Num));
			Render_Origin.h = (A_long)(Processvertex.x - ((PF_FpLong)checkedOutLayer.width* scaleTemp / 2));

			//Insert Time Code Here
			if (NeedsChkOut){
				TimeTmp = (FLOAT2FIX(params[TIMESHIFT]->u.fs_d.value * ((PF_FpLong)k / (PF_FpLong)Num)) >> 16)*(in_data->time_step);
				if (TimeTmp != TimeTmpPrev){
					//err = PF_CHECKIN_PARAM(in_data, &checkout);
					err = PF_CHECKOUT_PARAM(in_data, 1, CurrentChkoutTime + TimeTmp,
						in_data->time_step, in_data->time_scale, &checkout);
					checkedOutLayer = checkout.u.ld;
					if (err){
						suites.PathQuerySuite1()->PF_CheckinPath(in_data->effect_ref, params[PATHPARAM]->u.path_d.path_id, false, ThePath);
						return err;
					}
					TimeTmpPrev = TimeTmp;
				}	
			}

			Render_Origin.v = (A_long)(Processvertex.y - ((PF_FpLong)checkedOutLayer.height* scaleTemp / 2));
			CCU_SetIdentityMatrix(&transFormMatrix);
			CCU_RotateMatrixPlus(&transFormMatrix, in_data,
				angle + OrientAngle + AngleTemp, checkedOutLayer.width / 2, checkedOutLayer.height / 2);
			CCU_ScaleMatrix(&transFormMatrix, scaleTemp, scaleTemp, 0, 0);
			CCU_TransformPoints(&transFormMatrix, &Render_Origin);
			suites.WorldTransformSuite1()->transform_world(in_data->effect_ref, in_data->quality, PF_MF_Alpha_PREMUL, PF_Field_FRAME,
				&checkedOutLayer, &CompMode, NULL, &transFormMatrix, 1, true, &output->extent_hint, output);
			NowSegLength += DistBetween;

			//Insert Time Code Here(Checkin determination)

		}
		else{
			for (; k < Num; ++k){
				while (NowSegLength >= segmentLength->Value){
					++i;
					NowSegLength -= segmentLength->Value;
					segmentLength = segmentLength->next;
				}
				suites.PathDataSuite1()->PF_PathPrepareSegLength(in_data->effect_ref, ThePath, i, 10, &PrepPtr);
				if (params[AUTORIENT]->u.bd.value){
					suites.PathDataSuite1()->PF_PathEvalSegLengthDeriv1(in_data->effect_ref, ThePath, &PrepPtr, i, NowSegLength,
						&Processvertex.x, &Processvertex.y, &Processvertex.tan_in_x, &Processvertex.tan_in_y);
					OrientAngle = (suites.ANSICallbacksSuite1()->atan2(Processvertex.tan_in_y, Processvertex.tan_in_x)) / PF_RAD_PER_DEGREE;
				}
				else{
					suites.PathDataSuite1()->PF_PathEvalSegLength(in_data->effect_ref, ThePath, &PrepPtr, i, NowSegLength, &Processvertex.x, &Processvertex.y);
					OrientAngle = 0;
				}
				suites.PathDataSuite1()->PF_PathCleanupSegLength(in_data->effect_ref, ThePath, i, &PrepPtr);
				CompMode.opacity = (A_u_char)((PF_FpLong)OpacityTemp *((params[ALPHAATSTART]->u.fs_d.value)*(1 - ((PF_FpLong)k / ((PF_FpLong)Num - 1))) + (params[ALPHAATEND]->u.fs_d.value)*((PF_FpLong)k / ((PF_FpLong)Num - 1))));
				scaleTemp = scale*((params[SCALEATSTART]->u.fs_d.value)*(1 - ((PF_FpLong)k / ((PF_FpLong)Num - 1))) + (params[SCALEATEND]->u.fs_d.value)*((PF_FpLong)k / ((PF_FpLong)Num - 1)));
				AngleTemp = FIX_2_FLOAT(params[ROTATEATSTART]->u.ad.value) * (1 - ((PF_FpLong)k / ((PF_FpLong)Num - 1))) + FIX_2_FLOAT(params[ROTATEATEND]->u.ad.value) * ((PF_FpLong)k / ((PF_FpLong)Num - 1));

				//Insert Time Code Here
				if (NeedsChkOut){
					TimeTmp = (FLOAT2FIX(params[TIMESHIFT]->u.fs_d.value * ((PF_FpLong)k / (PF_FpLong)Num)) >> 16)*(in_data->time_step);
					if (TimeTmp != TimeTmpPrev){
						err = PF_CHECKIN_PARAM(in_data, &checkout);
						err = PF_CHECKOUT_PARAM(in_data, 1, CurrentChkoutTime + TimeTmp,
							in_data->time_step, in_data->time_scale, &checkout);
						checkedOutLayer = checkout.u.ld;
						if (err){
							suites.PathQuerySuite1()->PF_CheckinPath(in_data->effect_ref, params[PATHPARAM]->u.path_d.path_id, false, ThePath);
							return err;
						}
						TimeTmpPrev = TimeTmp;
					}
				}


				Render_Origin.h = (A_long)(Processvertex.x - ((PF_FpLong)checkedOutLayer.width* scaleTemp / 2));
				Render_Origin.v = (A_long)(Processvertex.y - ((PF_FpLong)checkedOutLayer.height* scaleTemp / 2));
				CCU_SetIdentityMatrix(&transFormMatrix);
				CCU_RotateMatrixPlus(&transFormMatrix, in_data,
					angle + OrientAngle + AngleTemp, checkedOutLayer.width / 2, checkedOutLayer.height / 2);
				CCU_ScaleMatrix(&transFormMatrix, scaleTemp, scaleTemp, 0, 0);
				CCU_TransformPoints(&transFormMatrix, &Render_Origin);
				suites.WorldTransformSuite1()->transform_world(in_data->effect_ref, in_data->quality, PF_MF_Alpha_PREMUL, PF_Field_FRAME,
					&checkedOutLayer, &CompMode, NULL, &transFormMatrix, 1, true, &output->extent_hint, output);
				NowSegLength += DistBetween;

				//Insert Time Code Here(Checkin determination)

			}
		}
	/*	break;
	case 2:
		for (j = Sum_length * params[STARTP]->u.fs_d.value / 100; j < EndPoint; j += params[DIST]->u.fs_d.value){
			while (NowSegLength >= segmentLength->Value){
				++i;
				NowSegLength -= segmentLength->Value;
				segmentLength = segmentLength->next;
			}
			suites.PathDataSuite1()->PF_PathPrepareSegLength(in_data->effect_ref, ThePath, i, 10, &PrepPtr);
			suites.PathDataSuite1()->PF_PathEvalSegLength(in_data->effect_ref, ThePath, &PrepPtr, i, NowSegLength, &Processvertex.x, &Processvertex.y);
			if (params[AUTORIENT]->u.bd.value){
				suites.PathDataSuite1()->PF_PathEvalSegLengthDeriv1(in_data->effect_ref, ThePath, &PrepPtr, i, NowSegLength, 
					&Processvertex.x, &Processvertex.y, &Processvertex.tan_in_x, &Processvertex.tan_in_y);
				OrientAngle = (suites.ANSICallbacksSuite1()->atan2(Processvertex.tan_in_y, Processvertex.tan_in_x)) / PF_RAD_PER_DEGREE;
			}
			else{
				suites.PathDataSuite1()->PF_PathEvalSegLength(in_data->effect_ref, ThePath, &PrepPtr, i, NowSegLength, &Processvertex.x, &Processvertex.y);
				OrientAngle = 0;
			}
			suites.PathDataSuite1()->PF_PathCleanupSegLength(in_data->effect_ref, ThePath, i, &PrepPtr);
			//suites.PathDataSuite1()->PF_PathVertexInfo(in_data->effect_ref, ThePath, i, &Processvertex);
			Render_Origin.h = (A_long)(Processvertex.x - ((PF_FpLong)checkedOutLayer.width* scale / 2));
			Render_Origin.v = (A_long)(Processvertex.y - ((PF_FpLong)checkedOutLayer.height* scale / 2));
			CCU_SetIdentityMatrix(&transFormMatrix);
			CCU_RotateMatrixPlus(&transFormMatrix, in_data, angle + OrientAngle, checkedOutLayer.width / 2, checkedOutLayer.height / 2);
			CCU_ScaleMatrix(&transFormMatrix, scale, scale, 0, 0);
			CCU_TransformPoints(&transFormMatrix, &Render_Origin);
			/*suites.WorldSuite1()->new_world(in_data->effect_ref, A_long((PF_FpLong)checkedOutLayer.width * scale)+1, A_long((PF_FpLong)checkedOutLayer.height * scale)+1, Worldflags, &AfterTransform);
			suites.Iterate8Suite1()->iterate_origin(in_data, 0, checkedOutLayer.height, &checkedOutLayer, &params[0]->u.ld.extent_hint, &Render_Origin, NULL, emitFUNC, output);
			suites.WorldTransformSuite1()->transfer_rect(in_data->effect_ref, in_data->quality, PF_MF_Alpha_PREMUL, PF_Field_FRAME, &checkedOutLayer.extent_hint, &checkedOutLayer, &CompMode, NULL, Render_Origin.h, Render_Origin.v, output);*/
			/*suites.WorldTransformSuite1()->transform_world(in_data->effect_ref, in_data->quality, PF_MF_Alpha_PREMUL, PF_Field_FRAME, &checkedOutLayer, 
				&CompMode, NULL, &transFormMatrix, 1, true, &output->extent_hint, output);
			/*suites.WorldTransformSuite1()->transfer_rect(in_data->effect_ref, in_data->quality, PF_MF_Alpha_PREMUL, PF_Field_FRAME, &AfterTransform.extent_hint, &AfterTransform, &CompMode, NULL, Render_Origin.x, Render_Origin.y, output);
			suites.WorldSuite1()->dispose_world(in_data->effect_ref, &AfterTransform);*/
			/*NowSegLength += params[DIST]->u.fs_d.value;
		}
		break;*/
	/*default:
		break;
	}*/
	if ((params[TRANSMODE]->u.pd.value == 2L) && (params[BACKG]->u.pd.value == 1L)){
		suites.WorldTransformSuite1()->composite_rect(in_data->effect_ref, NULL, 255, &params[0]->u.ld, 0, 0, PF_Field_FRAME, PF_Xfer_BEHIND, output);
	}
		err = PF_CHECKIN_PARAM(in_data, &checkout);
	suites.PathQuerySuite1()->PF_CheckinPath(in_data->effect_ref, params[PATHPARAM]->u.path_d.path_id, false, ThePath);
	/*Yes Ma'am!!*/

	return err;
}


DllExport	
PF_Err 
EntryPointFunc (
	PF_Cmd			cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extra)
{
	PF_Err		err = PF_Err_NONE;

	try {
		switch (cmd) {
			case PF_Cmd_USER_CHANGED_PARAM:
				err = ChangeParam(in_data,
					params,out_data,
					output, (PF_UserChangedParamExtra*)extra);
				break;
			case PF_Cmd_ABOUT:

				err = About(in_data,
							out_data,
							params,
							output);
				break;
				
			case PF_Cmd_GLOBAL_SETUP:

				err = GlobalSetup(	in_data,
									out_data,
									params,
									output);
				break;
				
			case PF_Cmd_PARAMS_SETUP:

				err = ParamsSetup(	in_data,
									out_data,
									params,
									output);
				break;
				
			case PF_Cmd_RENDER:

				err = Render(	in_data,
								out_data,
								params,
								output);
				break;
		}
	}
	catch(PF_Err &thrown_err){
		err = thrown_err;
	}
	return err;
}

