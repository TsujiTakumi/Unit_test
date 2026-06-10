/**
 * @file	apl_utility.c
 * @brief	アプリケーション共通ユーティリティ
 * @note	実行コンテキストに依存しない処理を記述<br>
 * 			このモジュールに配置する関数はスレッドセーフであること。
 * 			→グローバル変数による結合禁止<br>
 * @note	Dxygenコメントのつけ方[http://www.doxygen.jp/docblocks.html]
 * @date	2015-12-08 MSW S.Sugimoto 作成
 */

/*
 * --------------------------------------------------------------------------
 *   Header Include Area
 * --------------------------------------------------------------------------
 */
#include <stdlib.h>
#include <string.h>
#include "apl_main.h"
#include "apl_utility.h"
#include "apl_convert.h"
#include "apl_buf.h"
#include "apl_atlstc.h"
#include "../../shipment/shipment.h"
#include "../gui/event/hed/gui_event_engine.h"
#include "./common/model/apl_model_common_util.h"

/*
 * --------------------------------------------------------------------------
 *   Data Define/Local Declaration Area
 * --------------------------------------------------------------------------
 */

/*
 * --------------------------------------------------------------------------
 *   Style Define Area
 * --------------------------------------------------------------------------
 */
struct hex2dec
{
	UB hexstring_mc;
	UB dec_mc;
};

/*
 * --------------------------------------------------------------------------
 *   Variable Define Area
 * --------------------------------------------------------------------------
 */
/*!< 16進数文字を数値変換するテーブル	*/
const static struct hex2dec hex2dec_table_sxhR[] = {
	{	'0',	0x00	},
	{	'1',	0x01	},
	{	'2',	0x02	},
	{	'3',	0x03	},
	{	'4',	0x04	},
	{	'5',	0x05	},
	{	'6',	0x06	},
	{	'7',	0x07	},
	{	'8',	0x08	},
	{	'9',	0x09	},
	{	'a',	0x0a	},
	{	'b',	0x0b	},
	{	'c',	0x0c	},
	{	'd',	0x0d	},
	{	'e',	0x0e	},
	{	'f',	0x0f	},
	{	'A',	0x0a	},
	{	'B',	0x0b	},
	{	'C',	0x0c	},
	{	'D',	0x0d	},
	{	'E',	0x0e	},
	{	'F',	0x0f	},
};

/*!< 数値を16進数文字変換するテーブル	*/
const static UB dec2hex_table_schR[] = {
	'0',	// 0x00
	'1',	// 0x01
	'2',	// 0x02
	'3',	// 0x03
	'4',	// 0x04
	'5',	// 0x05
	'6',	// 0x06
	'7',	// 0x07
	'8',	// 0x08
	'9',	// 0x09
	'A',	// 0x0A
	'B',	// 0x0B
	'C',	// 0x0C
	'D',	// 0x0D
	'E',	// 0x0E
	'F',	// 0x0F
};

/* ↓↓↓↓↓↓↓↓ロゴ画像処理↓↓↓↓↓↓↓↓↓↓ */
/* ロゴ画像処理のステータス */
UB	ble_logo_process_stat_ec;

/* ロゴ画像受信データ保持領域 */
UB	*aplutil_logo_pic_data_ecp;
UH	aplutil_logo_pic_data_size_eu;
UH	aplutil_logo_pic_data_read_index_eu;

/* ロゴ画像１圧縮単位切り出し領域 */
ble_hotel_logo_pic_comp_unit_data_t		ble_hotel_logo_pic_comp_unit_buf_ex;

/* ロゴ画像解凍領域 */
ble_hotel_logo_pic_decomp_data_t		ble_hotel_logo_pic_decomp_buf_ex;

/* ロゴ画像書き込み回数：FLASH書込み時のIndex指定に使う */
UW	ble_logo_save_count_el;

/* ロゴ画像書き込み進捗率(%) */
UB	ble_logo_save_rate_ec;
/* ↑↑↑↑↑↑↑ロゴ画像処理↑↑↑↑↑↑↑↑↑↑ */

/* BLE受信種別の処理 */
typedef struct blecomtype_cnv_guievent{
	UB blecomtype_mc;		/* セッション種別 */
	UB guievent_mc;		/* フェーズ種別 */
}blecomtype_cnv_guievent_t;

const blecomtype_cnv_guievent_t blecomtype_cnv_guievent_tbl[BLECOM_SESSSIONPHASETYPE_MAX]={
	/* BLE受信種別											GUIイベント			*/
	/*blecomtype_mc										guievent_mc			*/
 	{ BLECOM_SESSSIONPHASETYPE_LOGIN_START				,	IMM_EV_BLE_LOGIN_START			},		/*!< ログインセッションの開始要求                                            */
 	{ BLECOM_SESSSIONPHASETYPE_LOGIN_END				,	IMM_EV_BLE_LOGIN_END			},		/*!< ログインセッション終了(セッションの正常終了)                            */
 	{ BLECOM_SESSSIONPHASETYPE_LOGOUT_START				,	IMM_EV_BLE_LOGOUT_START			},		/*!< ログアウトセッションの開始要求                                          */
 	{ BLECOM_SESSSIONPHASETYPE_LOGOUT_END				,	IMM_EV_BLE_LOGOUT_END			},		/*!< ログアウトセッション終了(セッションの正常終了)                          */
 	{ BLECOM_SESSSIONPHASETYPE_GUEST_START				,	IMM_EV_BLE_GUEST_START			},		/*!< ゲストセッションの開始要求                                              */
 	{ BLECOM_SESSSIONPHASETYPE_GUEST_END				,	IMM_EV_BLE_GUEST_END			},		/*!< ゲストセッション終了(セッションの正常終了)                              */
 	{ BLECOM_SESSSIONPHASETYPE_TYPE_START				,	IMM_EV_BLE_TYPE_START			},		/*!< 機種情報セッションの開始要求                                            */
 	{ BLECOM_SESSSIONPHASETYPE_TYPE_END					,	IMM_EV_BLE_TYPE_END				},		/*!< 機種情報セッション終了(セッションの正常終了)                            */
 	{ BLECOM_SESSSIONPHASETYPE_MENTE_FUNC_START			,	IMM_EV_BLE_MENTE_FUNC_START		},		/*!< メンテナンス(機能選択中あり)セッションの開始要求イベント                */
 	{ BLECOM_SESSSIONPHASETYPE_MENTE_FUNC_END			,	IMM_EV_BLE_MENTE_FUNC_END		},		/*!< メンテナンス(機能選択中あり)セッションの終了要求イベント(正常終了)      */
 	{ BLECOM_SESSSIONPHASETYPE_NON						,	IMM_EV_BLE_MENTE_FUNC_NON_START	},		/*!< メンテナンス(機能選択中なし)セッションの開始要求イベント                */  //メンテナンス(機能選択中なし)セッション中はメンテナンス描画更新通知受信時に画面遷移させる。
 	{ BLECOM_SESSSIONPHASETYPE_NON						,	IMM_EV_BLE_MENTE_FUNC_NON_END	},		/*!< メンテナンス(機能選択中なし)セッションの終了要求イベント(正常終了)      */  //メンテナンス(機能選択中なし)セッション中はメンテナンス描画更新通知受信時に画面遷移させる。
 	{ BLECOM_SESSSIONPHASETYPE_BUTTON_OPE_START			,	IMM_EV_BLE_BUTTON_OPE_START		},		/*!< ボタン操作セッションの開始要求                                          */
 	{ BLECOM_SESSSIONPHASETYPE_BUTTON_OPE_END			,	IMM_EV_BLE_BUTTON_OPE_END		},		/*!< ボタン操作セッション終了(セッションの正常終了)                          */
 	{ BLECOM_SESSSIONPHASETYPE_PICT_UPDATE_START		,	IMM_EV_BLE_PICT_UPDATE_START	},		/*!< ロゴ画像更新セッションの開始要求                                        */
 	{ BLECOM_SESSSIONPHASETYPE_PICT_UPDATE_END			,	IMM_EV_BLE_PICT_UPDATE_END		},		/*!< ロゴ画像更新セッション終了(セッションの正常終了)                        */
 	{ BLECOM_SESSSIONPHASETYPE_PALLET_UPDATE_START		,	IMM_EV_BLE_PALLET_UPDATE_START	},		/*!< カラーパレット更新セッションの開始要求                                  */
 	{ BLECOM_SESSSIONPHASETYPE_PALLET_UPDATE_END		,	IMM_EV_BLE_PALLET_UPDATE_END	},		/*!< カラーパレット更新セッション終了(セッションの正常終了)                  */
};


const UB aucBleMenteExeResultMngTbl_sch[BLE_MENTE_COLLECT_RESULT_MAX] = 
{
	BLE_MENTE_COLLECT_RESULT_CANCEL					,   /* 中断                      */
	BLE_MENTE_COLLECT_RESULT_EXCEPT_BLE_DISCONNECT	,   /* 異常終了(BLE通信継続不可) */
	BLE_MENTE_COLLECT_RESULT_EXCEPT_BLE_CONNECT		,   /* 異常終了(BLE通信継続可)   */
	BLE_MENTE_COLLECT_RESULT_NOT					,   /* 未対応                    */
	BLE_MENTE_COLLECT_RESULT_COMP					,   /* 正常終了                  */
	BLE_MENTE_COLLECT_RESULT_NON					,   /* 未完了                    */
};

#ifdef USE_BLE
//↓48MA_遠隔用パスワードの仕様変更対応_Technosite_maruyama_20250923
/* MCLL認証パスワード変換テーブル */
const static UB convert_bdadr2mcll_table_sch[CONVERT_REMOTE_PW_NUM_MAX] = {
	0x07, 0x0C, 0x05, 0x0F, 0x01, 0x0D, 0x04, 0x08, 0x0E, 0x02, 0x09, 0x06, 0x00, 0x0A, 0x03, 0x0B
};

/* GW認証パスワード変換テーブル */
const static UB convert_bdadr2gw_table_sch[CONVERT_REMOTE_PW_NUM_MAX] = {
	0x0A, 0x09, 0x0F, 0x08, 0x0C, 0x03, 0x00, 0x04, 0x06, 0x0E, 0x02, 0x0D, 0x05, 0x0B, 0x01, 0x07		//48MA_GW認証パスワード参照誤り修正_Technosite_maruyama_20251016
};
//↑48MA_遠隔用パスワードの仕様変更対応_Technosite_maruyama_20250923
#endif //USE_BLE

// Kawa181115 start
// 電力量収集機能追加
#ifdef MODEL_MA
static const struct data_elecenergy default_data_elecenergy_sxR = {ELECENERGY_DEC_NONE, ELECENERGY_INT_NONE};
#else	// MODEL_ME
#endif	// MODEL_MA
// Kawa181115 end

/*
 * --------------------------------------------------------------------------
 *   Prototype Declaration Area
 * --------------------------------------------------------------------------
 */
#ifdef COMPACT_MA
static UB aplutl_chg_bsp2apl_calibdata_fc(T_CALIB_DATA bsp_rx, struct calibration_set *apl_rxp);
static UB aplutl_chg_apl2bsp_calibdata_fc(struct calibration_set apl_rx, T_CALIB_DATA *bsp_rxp);
#else	// SMART_MA or SMART_ME
#endif	// COMPACT_MA
#ifdef USE_BLE
static unsigned long	natol_fl( UB *chars_scp, UB size_rc );		/* 文字列数値変換 */	// [BLE] 20170327 Miyazaki
UB	aplutl_calc_logo_pic_process_progress_rate_fc( UW count_rl );
void	aplutl_set_logo_pic_process_progress_start_fv( void );
void	aplutl_set_logo_pic_process_progress_rate_fv( UB rate_rc );
#endif //USE_BLE
static UB judge_pri_BleMenteExeResult_fc(UB ucSpecifyResult_rc);
static void aplutl_judge_vent_ctrl_if_info_fv(UB *vent_ctrl_if_switch_rc, UB *vent_ctrl_if_ope_rc);	//↓46MA 換気機器選択対応_MESW_Masuda_20231020
static UB aplutl_judge_vent_ctrl_if_switch_fc(UB vent_if_rc, UB lossnay_rc, UB model_rc);
static UB aplutl_judge_vent_ctrl_if_ope_fc(UB vent_if_rc, UB lossnay_rc, UB model_rc);				//↑46MA 換気機器選択対応_MESW_Masuda_20231020
#ifdef USE_BLE
static UH aplutl_convert_bdadr2mcll_fu(UB *bdadr_rcp);		//48MA_遠隔用パスワードの仕様変更対応_Technosite_maruyama_20250923
static UH aplutl_convert_bdadr2gw_fu(UB *bdadr_rcp);		//48MA_遠隔用パスワードの仕様変更対応_Technosite_maruyama_20250923
#endif //USE_BLE

/**
 * @ingroup		Framework
 * @brief		クイックソート
 * @param[in]	base				-	配列の先頭
 * @param[in]	nelem				-	配列要素数
 * @param[in]	size				-	１個の配列のサイズ
 * @param[in]	cmp					-	比較関数
 * @note		36MAより移植
 * @warning		
 * @date		2015.12.8
 * @author		MSW S.Sugimoto
 */
void sort_fv(void *base, unsigned int nelem, unsigned int size,	int (*cmp)(void  *el, void *e2))
{
	/*++クイックソートを実施する*/
	int		m, n, i;
	char	tmp;
	for(m = 0; m != nelem; m++) {
		for(n = m; n != nelem; n++) {
			if(cmp((char *)base + m * size, (char *)base + n * size) > 0) {
				for(i = 0; i != size; i++) {
					tmp = *((char *)base + m * size + i);
					*((char *)base + m * size + i) = *((char *)base + n * size + i);
					*((char *)base + n * size + i) = tmp;
				}
			}
		}
	}
	/*--*/
}

/**
 * @ingroup		Framework
 * @brief		アプリケーションの管理するメッセージバッファへの送信
 * @param[in]	*buffer_rx				-	送信データ
 * @return		snd_mbfの戻り値をそのまま返す
 * @note		
 * @warning		タスクコンテキストからのみ実行可能
 * @date		2015.12.8
 * @author		MSW S.Sugimoto
 */
ER snd_message_fc(struct apl_buf* buffer_rx)
{
	/*++アプリケーションにメッセージバッファを送信する*/
	ER rtn = E_OK;
	rtn = snd_mbf(ID_apl_message_buffer,(VP)buffer_rx,sizeof(struct apl_buf));
	/*--*/
	/*++実行結果を返却する*/
	return rtn;
	/*--*/
}

/**
 * @ingroup		Framework
 * @brief		アプリケーションの管理するメッセージバッファへの送信（割り込み用）
 * @param[in]	*buffer_rx				-	送信データ
 * @return		snd_mbfの戻り値をそのまま返す
 * @note		
 * @warning		割り込みコンテキストからのみ実行可能
 * @date		2015.12.8
 * @author		MSW S.Sugimoto
 */
ER isnd_message_fc(struct apl_buf* buffer_rx)
{
	/*++アプリケーションにメッセージバッファを送信する（割り込み用）*/
	ER rtn = E_OK;
	rtn = ipsnd_mbf(ID_apl_message_buffer,(VP)buffer_rx,sizeof(struct apl_buf));
	/*--*/
	/*++実行結果を返却する*/
	return rtn;
	/*--*/
}

/**
 * @ingroup		Framework
 * @brief		引数の要求IDから実行する関数を検索し、実行する
 * @param[in]	request_id_rc - 要求ID
 * @param[in]	src_id_rc - 要求元モジュール
 * @param[in]	*data - 要求データ
 * @param[in]	*table_rxp - 関数テーブル
 * @param[in]	table_size_rx - 関数テーブルサイズ
 * @return		制御実行結果
 * @note		
 * @warning		
 * @date		2016.1.25
 * @author		MSW S.Sugimoto
 */
UB jump_function_fc(UH request_id_rc ,APL_SRCMODULE_ID_T src_id_rc, union data *data,struct function_tbl *table_rxp,UH table_size_rx)
{
	/*++引数の要求IDから実行する関数を検索し、対象の関数にジャンプする*/
	UH loop_ac;
	UB rtn = APL_FALSE;
	/*++関数テーブルを検索し、要求IDが有効かどうかチェックする*/
	for( loop_ac = 0 ; loop_ac < table_size_rx ; loop_ac++ ) {
		if( table_rxp[loop_ac].request_id == request_id_rc ) {
			if( table_rxp[loop_ac].function_mx != NULL ) {
				rtn = table_rxp[loop_ac].function_mx(src_id_rc,data);
			}
			break;
		}
	}
	/*--*/
	/*--*/
	/*++実行結果を返却する*/
	return rtn;
	/*--*/
}

#ifdef ENABLE_THROW_EXCEPTION
/**
 * @ingroup		Framework
 * @brief		例外を発行する
 * @param[in]	*file - ファイル名(__FILE__マクロの内容)
 * @param[in]	line - __LINE__を設定
 * @return		なし
 * @note		ENABLE_THROW_EXCEPTIONを有効にした場合のみ関数が有効になります。
 * @warning		割り込みからは実行禁止
 * @date		2016.1.25
 * @author		MSW S.Sugimoto
 */
void ithrow_exception(UB *file , UH line)
{
	/*++例外検出時、メッセージを発行する（割り込み用）*/
	/* ENABLE_THROW_EXCEPTIONが有効なとき、この関数は有効 */
	UB loop;
	struct apl_buf buffer_ax;
	buffer_ax.data_mx.exception.file = file;
	buffer_ax.data_mx.exception.line = line;
	buffer_ax.pattern_mi = EXCEPTION;
	/*++例外検出したファイル・行を出力する*/
	for( loop = 0 ; loop < 10 ; loop++){
		if( E_OK != isnd_message_fc(&buffer_ax) ){
			dly_tsk(5);
		} else {
			break;
		}
	}
	/*--*/
	/*--*/
}

/**
 * @ingroup		Framework
 * @brief		例外を発行する
 * @param[in]	*file - ファイル名(__FILE__マクロの内容)
 * @param[in]	line - __LINE__を設定
 * @return		なし
 * @note		ENABLE_THROW_EXCEPTIONを有効にした場合のみ関数が有効になります。
 * @warning		タスクコンテキストからは実行禁止
 * @date		2016.1.25
 * @author		MSW S.Sugimoto
 */
void throw_exception(UB *file , UH line)
{
	/*++例外検出時、メッセージを発行する*/
	/* ENABLE_THROW_EXCEPTIONが有効なとき、この関数は有効 */
	UB loop;
	struct apl_buf buffer_ax;
	buffer_ax.data_mx.exception.file = file;
	buffer_ax.data_mx.exception.line = line;
	buffer_ax.pattern_mi = EXCEPTION;
	/*++例外検出したファイル・行を出力する*/
	for( loop = 0 ; loop < 10 ; loop++){
		if( E_OK != snd_message_fc(&buffer_ax) ){
			dly_tsk(50);
		} else {
			break;
		}
	}
	/*--*/
	/*--*/
}
#endif

/**
 * @ingroup		application
 * @brief		曜日計算
 * @param[in]	year_rc 年( 1byte BSD)
 * @param[in]	month_rc 月( 1byte BSD)
 * @param[in]	day_rc 日( 1byte BSD)
 * @return		snd_mbfの戻り値をそのまま返す
 * @note		
 * @warning		割り込みコンテキストからのみ実行可能
 * @date		2015.12.8
 * @author		MSW S.Sugimoto
 */
UB util_calculation_week(UB year_rc,UB month_rc,UB day_rc)
{
	/*++曜日を計算する*/
	W	year_al;		// [A_026 Miyazaki 080726]
	W	month_al;		// [A_026 Miyazaki 080726]
	W	day_al;			// [A_026 Miyazaki 080726]
	UW week_al;
	
	/*++年月日をBCD値→BIN値に変換する*/
	/* 曜日の計算 */
	year_al  = (W)( bcd2bin(year_rc) );
	month_al = (W)( bcd2bin(month_rc) );
	day_al   = (W)( bcd2bin(day_rc) );
	/*--*/
	/* 年を計算用に補正する */
	year_al = 2000 + year_al;						//[A_040 honda 080909]20000年の演算用に補正
	/* 1月、2月は前の年の13月、14月として計算する */
	if( ( 1 == month_al ) || ( 2 == month_al ) ){
		year_al--;
		month_al += 12;
	}
	/* 年月日を基に曜日を計算する */
	week_al = ( year_al + ( year_al / 4 ) - ( year_al / 100 ) + ( year_al / 400 ) + ( ( ( 13 * month_al ) + 8 ) / 5 ) + day_al ) % 7;
	/*--*/
	/*++実行結果を返却する*/
	return (UB)week_al;
	/*--*/
}

/**
 * @ingroup		application
 * @brief		BCD→BIN変換
 * @param[in]	bcd_rc BCD値
 * @return		BIN値
 * @note		
 * @warning		
 * @date		2015.12.8
 * @author		MSW S.Sugimoto
 */
UB bcd2bin(UB bcd_rc) {
	/*++BCD値をBIN値に変換して返却する*/
	return ( (bcd_rc & 0x0f) + ((bcd_rc & 0xf0)>>4) * 10);
	/*--*/
}

/**
 * @ingroup		application
 * @brief		BIN→BCD変換
 * @param[in]	bin_rc BIN値
 * @return		BCD値
 * @note		
 * @warning		
 * @date		2015.12.8
 * @author		MSW S.Sugimoto
 */
UB bin2bcd(UB bin_rc) {
	/*++BIN値をBCD値に変換して返却する*/
	return ( (bin_rc % 10) + ((bin_rc % 100)/10) * 0x10);
	/*--*/
}

/**
 * @ingroup 	Framework
 * @brief		16進数文字列を数値変換する
 * @note		
 * @eturn		APL_TRUE：成功 APL_FALSE：変換失敗
 * @warning		
 * @date		2016.12.12
 * @author		MSW K.Kawamura
 */
UB hexchar2dec_fc(UB c_rc, UB *result_rcp)
{
	UB i_ac = 0;
	UB rtn_ac = APL_FALSE;

	/*引数チェック*/
	if(NULL != result_rcp){
		*result_rcp = 0;
		/*1文字の16進数文字を数値に変換する*/
		for(i_ac = 0 ; (sizeof(hex2dec_table_sxhR) / sizeof(struct hex2dec)) > i_ac ; i_ac++){
			if(hex2dec_table_sxhR[i_ac].hexstring_mc == c_rc){
				*result_rcp = hex2dec_table_sxhR[i_ac].dec_mc;
				rtn_ac = APL_TRUE;
				break;
			}
		}
	}
	return rtn_ac;
}

/**
 * @ingroup 	Framework
 * @brief		数値を16進数文字列変換する
 * @note		
 * @eturn		APL_TRUE：成功 APL_FALSE：変換失敗
 * @warning		
 * @date		2016.12.12
 * @author		MSW K.Kawamura
 */
UB dec2hexchar_fc(UB d_rc, UB *result_rcp)
{
	UB rtn_ac = APL_FALSE;

	/*引数チェック*/
	if(NULL != result_rcp){
		*result_rcp = ' ';
		/*数値を1文字の16進数文字に変換する*/
		if(0x10 > d_rc){
			*result_rcp = dec2hex_table_schR[d_rc];
			rtn_ac = APL_TRUE;
		}
	}
	return rtn_ac;
}

/**
 * @ingroup		application
 * @brief		異常履歴へ異常データを追加する
 * @param[out]	*error - 追加する異常データへのポインタ
 * @note		
 * @warning		
 * @date		2015.12.8
 * @author		MSW S.Sugimoto
 */
void aplutl_add_errlog(struct error *error)
{
	/*++異常履歴へ異常データを追加する*/
	UB					n;
	struct err_log		err_log;
//	UB					clock_state;
//	struct clock		clock;
	
//43MA_異常履歴範囲拡大対応 msw_nagasi_20200924_start 追加条件削除
////43MA_MA通信異常対策対応 msw_nagasi_20200904	/*++異常(自己検出異常は除く)の場合のみ、異常履歴を更新する*/
////43MA_MA通信異常対策対応 msw_nagasi_20200904	if((error->err_mente == ERROR_ERROR)						//[A_031 NS_KOYAMA 080809]
////43MA_MA通信異常対策対応 msw_nagasi_20200904	&& (error->atr != ERRATR_SELF)) {							//[A_031 NS_KOYAMA 080809]属性0xffは内部異常、履歴に追加しない
////43MA_MA通信異常対策対応 msw_nagasi_20200904_start
//	// ++異常(自己検出異常は除く)の場合もしくは
//	// ＜４桁異常コード＞が【0x6831】～【0x6834】の場合、異常履歴を更新する
//	if( ((error->err_mente == ERROR_ERROR)
//	 &&  (error->atr != ERRATR_SELF))		// 自己検出異常<属性0xff>は内部異常、履歴に追加しない
//	 || (0x6831 == error->codelong)
//	 || (0x6832 == error->codelong)
//	 || (0x6833 == error->codelong)
//	 || (0x6834 == error->codelong) ) {
////43MA_MA通信異常対策対応 msw_nagasi_20200904_end
//43MA_異常履歴範囲拡大対応 msw_nagasi_20200924_end
		for(n = 0; n != ERRLOG_MAX; n++) {
			read_db_err_log_fc(n, &err_log);
			if(err_log.storage_flg == UNDEFINED) {				//[A_030 NS_KOYAMA 080804]
				break;
			}
		}
		if(n >= ERRLOG_MAX) {
			/* 異常履歴追加不可(余裕なし) */
			/* 異常履歴（０～１４）←異常履歴（１～１４）全体をシフト */
			for(n = 0; n != ERRLOG_MAX-1; n++) {
				read_db_err_log_fc(n+1, &err_log);
				write_db_err_log_fc(n, &err_log);
			}

			n = ERRLOG_MAX - 1;
		}
//start [37MA A01 chata 20151222]	
//		/* 異常履歴追加 */
//		read_db_isclockset_fc(&clock_state);						//[A_030 NS_KOYAMA 080804]
//		if(clock_state == CLOCK_INI) {							//[A_030 NS_KOYAMA 080804]
//			/* 時計時刻未設定 */								//[A_030 NS_KOYAMA 080804]
//			err_log.date.day = UNDEFINED;						//[A_030 NS_KOYAMA 080804]
//			err_log.date.month = UNDEFINED;						//[A_030 NS_KOYAMA 080804]
//			err_log.date.year = UNDEFINED;						//[A_030 NS_KOYAMA 080804]
//			err_log.time.hour = UNDEFINED;						//[A_030 NS_KOYAMA 080804]
//			err_log.time.min = UNDEFINED;						//[A_030 NS_KOYAMA 080804]
//		} else {												//[A_030 NS_KOYAMA 080804]
//			read_db_clock_fc(&clock);
//			err_log.date = clock.date;
//			err_log.time = clock.time;
//		}														//[A_029 NS_KOYAMA 080804]
		//日にちを設定
		err_log.date = error->date;
		//時間を設定
		err_log.time = error->time;
//end [37MA A01 chata 20151222]	
		err_log.storage_flg = !UNDEFINED;						//[A_030 NS_KOYAMA 080805]
		err_log.adr = error->unitadr;
		err_log.goki = error->unitgoki;
		err_log.codeshort = error->codeshort;
		err_log.codelong = error->codelong;
//20201002_異常履歴範囲拡大_43MA_chata_start
		err_log.err_mente_mc = error->err_mente;				/* 異常種別(異常orメンテ)			*/	
	
		//属性、検出方法を設定する。
		if(ERRATR_SELF == error->atr){
			//検出方法が自己検出の場合、属性を"RC"、検出方法を"自己検出"とする。
			err_log.detection_method_mc = ERR_DETECTION_SELF; 
			err_log.atr_mc = ERRATR_RC;
		}else{
			//検出方法が通報の場合、属性を"受信値"、検出方法を"通報"とする。
			
			err_log.detection_method_mc = ERR_DETECTION_RECEIVE;
			err_log.atr_mc = error->atr;
		}
//20201002_異常履歴範囲拡大_43MA_chata_end
	
		write_db_err_log_fc(n, &err_log);

		/* 不揮発記憶要求あり */
		updateinfo.eewrreq = 1;
//43MA_異常履歴範囲拡大対応 msw_nagasi_20200924	}
	/*--*/
	/*--*/
}

//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
/**
 *	@brief		異常時刻取得
 *	@param[out]	*error_rxp - 追加する異常データへのポインタ
 *	@return		なし
 *	@warning    
 *	@note		[37MA A01 chata 20151222]
 **/
//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
void aplutl_get_errclock_fv(struct error *error_rxp)
{
	/*++異常時刻を取得する*/
	UB					clock_state;
	struct clock		clock;
	
	/*++時刻設定状態を読み出す*/
	read_db_isclockset_fc(&clock_state);
	/*--*/
	/*++時刻設定済みかチェックする*/
	if(clock_state == CLOCK_INI) {		/* 時計時刻未設定 */
		/*++異常時刻を未設定とする*/
		error_rxp->date.day = UNDEFINED;
		error_rxp->date.month = UNDEFINED;
		error_rxp->date.year = UNDEFINED;
		error_rxp->time.hour = UNDEFINED;
		error_rxp->time.min = UNDEFINED;
		/*--*/
	} else {							/* 時計時刻設定済み */
		/*++現在時刻を設定する*/
		read_db_clock_fc(&clock);
		error_rxp->date = clock.date;
		error_rxp->time = clock.time;
		/*--*/
	}
	/*--*/
	/*--*/
}

// ↓国内版43MA_異常猶予履歴対応_inoue_20200924
/**
 * @ingroup		application
 * @brief		異常猶予履歴へ異常猶予情報を追加する
 * @param[out]	*grace_error_rxp - 追加する異常猶予情報へのポインタ
 * @note		
 * @warning		
 * @date		2020.09.24
 * @author		MSW Inoue
 */
void aplutl_add_grace_errlog_fv(struct grace_error *grace_error_rxp)
{
	/*++ 変数を宣言する */
	UB	rw_pos_ac;
	UB	end_pos_ac;
	struct gra_err_log	grace_err_log_rx;
	/*--*/

	/*++ 異常無し以外は履歴へ追加する */
	if(ERROR_NONE != grace_error_rxp->err_mente) {
		/*++ 異常猶予履歴が格納されていない位置を探す */
		for(rw_pos_ac = 0; GRACE_ERRLOG_MAX > rw_pos_ac; rw_pos_ac++) {
			read_db_grace_err_log_fc(rw_pos_ac, &grace_err_log_rx);
			if(UNDEFINED == grace_err_log_rx.storage_flg) {
				break;
			}
		}
		/*--*/
		/*++ 最大件数を超える情報は古い履歴を削除して詰める */
		if(GRACE_ERRLOG_MAX <= rw_pos_ac) {
			/* 異常猶予履歴追加不可(余裕なし) */
			/* 異常猶予履歴全体を1つシフトする */
			end_pos_ac = GRACE_ERRLOG_MAX - 1;
			for(rw_pos_ac = 0; end_pos_ac > rw_pos_ac; rw_pos_ac++) {
				read_db_grace_err_log_fc(rw_pos_ac+1, &grace_err_log_rx);
				write_db_grace_err_log_fc(rw_pos_ac, &grace_err_log_rx);
			}
			/* 末尾を書き込み位置とする */
			rw_pos_ac = end_pos_ac;
		}
		/*--*/
		/*++ 異常猶予履歴を作成する */
		/* 格納済みとする */
		grace_err_log_rx.storage_flg	= !UNDEFINED;
		/* 異常猶予情報を取得する */
		grace_err_log_rx.date			= grace_error_rxp->date;
		grace_err_log_rx.time			= grace_error_rxp->time;
		grace_err_log_rx.adr			= grace_error_rxp->unitadr;
		grace_err_log_rx.goki			= grace_error_rxp->unitgoki;
		grace_err_log_rx.codeshort		= grace_error_rxp->codeshort;
		grace_err_log_rx.codelong		= grace_error_rxp->codelong;
		/*--*/
		/* 異常猶予履歴へ書き込む */
		write_db_grace_err_log_fc(rw_pos_ac, &grace_err_log_rx);

		/* 不揮発記憶要求あり */
		updateinfo.eewrreq = APL_TRUE;
		
		/* 描画更新要求あり */
		updateinfo.disp_chg = APL_TRUE;
	}
	/*--*/
}

//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
/**
 *	@brief		異常猶予時刻取得
 *	@param[out]	*error_rxp - 追加する異常データへのポインタ
 *	@return		なし
 *	@warning    
 *	@note
 **/
//■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
void aplutl_get_grace_errclock_fv(struct grace_error *grace_error_rxp)
{
	/*++異常猶予時刻を取得する*/
	UB					clock_state;
	struct clock		clock;
	
	/*++時刻設定状態を読み出す*/
	read_db_isclockset_fc(&clock_state);
	/*--*/
	/*++時刻設定済みかチェックする*/
	if(clock_state == CLOCK_INI) {		/* 時計時刻未設定 */
		/*++異常時刻を未設定とする*/
		grace_error_rxp->date.day = UNDEFINED;
		grace_error_rxp->date.month = UNDEFINED;
		grace_error_rxp->date.year = UNDEFINED;
		grace_error_rxp->time.hour = UNDEFINED;
		grace_error_rxp->time.min = UNDEFINED;
		/*--*/
	} else {							/* 時計時刻設定済み */
		/*++現在時刻を設定する*/
		read_db_clock_fc(&clock);
		grace_error_rxp->date = clock.date;
		grace_error_rxp->time = clock.time;
		/*--*/
	}
	/*--*/
	/*--*/
}
// ↑国内版43MA_異常猶予履歴対応_inoue_20200924

/**
 * @ingroup		application
 * @brief		二捨三入七捨八入
 * @param[in]		num - 計算数値（10倍値（25.5℃ → 255））
 * @return		計算結果（10倍値（25.5℃ → 255））
 * @note		
 * @warning		
 * @date		2016.1.29
 * @author		Reiden Tomomatsu
 */
H rounding_2off3up7off8up(H num)
{
	/*++二捨三入、七捨八入を実行する*/
	H clc;
	
	if(num >= 0) {
		clc = (num + 2) / 5;
	} else {
		clc = (num - 2) / 5;
	}
	clc = clc * 5;
	/*--*/
	/*++計算結果を返却する*/
	return clc;
	/*--*/
}

/**
 * @ingroup		application
 * @brief		四捨五入
 * @param[in]		num - 計算数値（10倍値（25.5℃ → 255））
 * @return		計算結果（10倍値（25.5℃ → 255））
 * @note		
 * @warning		
 * @date		2016.1.29
 * @author		Reiden Tomomatsu
 */
H rounding_4off5up(H num)
{
	/*++四捨五入を実行する*/
	H clc;
	
	if(num >= 0) {
		clc = (num + 5) / 10;
	} else {
		clc = (num - 5) / 10;
	}
	clc = clc * 10;
	/*--*/
	/*++計算結果を返却する*/
	return clc;
	/*--*/
}

/**
 * @ingroup		application
 * @brief		0.5℃単位切り下げ
 * @param[in]		num - 計算数値（10倍値（25.5℃ → 255））
 * @return		計算結果（10倍値（25.5℃ → 255））
 * @note		
 * @warning		
 * @date		2016.1.29
 * @author		Reiden Tomomatsu
 */
H rounding_05unit_off(H num)
{
	/*++0.5℃単位切り下げを実行する*/
	H clc;
	clc = (num) / 5;
	clc = clc * 5;
	/*--*/
	/*++計算結果を返却する*/
	return clc;
	/*--*/
}

/**
 * @ingroup		application
 * @brief		1℃単位切り下げ
 * @param[in]		num - 計算数値（10倍値（25.5℃ → 255））
 * @return		計算結果（10倍値（25.5℃ → 255））
 * @note		
 * @warning		
 * @date		2016.1.29
 * @author		Reiden Tomomatsu
 */
H rounding_10unit_off(H num)
{
	/*++1℃単位切り下げを実行する*/
	H clc;
	clc = (num) / 10;
	clc = clc * 10;
	/*--*/
	/*++計算結果を返却する*/
	return clc;
	/*--*/
}

/**
 * @ingroup		application
 * @brief		0.5℃単位切り上げ
 * @param[in]		num - 計算数値（10倍値（25.5℃ → 255））
 * @return		計算結果（10倍値（25.5℃ → 255））
 * @note		
 * @warning		
 * @date		2016.1.29
 * @author		Reiden Tomomatsu
 */
H rounding_05unit_up(H num)
{
	/*++0.5℃単位切り上げを実行する*/
	H clc;
	
	if(num >= 0) {
		clc = (num + 4) / 5;
	} else {
		clc = (num - 4) / 5;
	}
	clc = clc * 5;
	/*--*/
	/*++計算結果を返却する*/
	return clc;
	/*--*/
}

/**
 * @ingroup		application
 * @brief		1℃単位切り上げ
 * @param[in]		num - 計算数値（10倍値（25.5℃ → 255））
 * @return		計算結果（10倍値（25.5℃ → 255））
 * @note		
 * @warning		
 * @date		2016.1.29
 * @author		Reiden Tomomatsu
 */
H rounding_10unit_up(H num)
{
	/*++1℃単位切り上げを実行する*/
	H clc;
	
	if(num >= 0) {
		clc = (num + 9) / 10;
	} else {
		clc = (num - 9) / 10;
	}
	clc = clc * 10;
	/*--*/
	/*++計算結果を返却する*/
	return clc;
	/*--*/
}

/**
 * @ingroup		application
 * @brief		BCD→BIN変換
 * @param[in]	bcd_ru - BCD値
 * @return		BIN値
 * @note		
 * @warning		
 * @date		2017.8.29
 * @author		MSW Kawamura
 */
UH ev_Bcd4Bin_fu(UH bcd_ru){
	/*++BCD値をBIN値に変換して返却する*/
	return ((bcd_ru & 0x000f) +
			((bcd_ru & 0x00f0) >> 4) * 10 +
			((bcd_ru & 0x0f00) >> 8) * 100 +
			((bcd_ru & 0xf000) >> 12) * 1000);
	/*--*/
}

/**
 * @ingroup		application
 * @brief		BIN→BCD変換
 * @param[in]	bin_ru - BIN値
 * @return		- BCD値
 * @note		
 * @warning		
 * @date		2016.2.11
 * @author		Reiden Tomomatsu
 */
UH ev_Bin4Bcd_fu(UH bin_ru) {
	/*++BIN値をBCD値に変換して返却する(４桁)*/
	return ( (bin_ru % 10)
		+ ((bin_ru % 100)/10) * 0x10
		+ ((bin_ru % 1000)/100) * 0x100
		+ ((bin_ru % 10000)/1000) * 0x1000);
	/*--*/
}

/****************************************************************************
 *	設定温度自動復帰タイマー＜080428追加＞
 ****************************************************************************/
/*****************************************************************
 *	設定温度自動復帰タイマー起動
 *	パラメータ：なし
 *	リターン  ：なし
 *	機能：	下記条件をすべて満足する場合、ＤＢの設定温度自動復帰時間をアラームに設定（起動）する。
 *			・設定温度自動復帰有効
 *			・ＤＢの運転/停止が運転
 *			・(冷房and室温設定＜自動復帰冷房温度)or(暖房and室温設定＞自動復帰暖房温度)
 *****************************************************************/
/**
 * @ingroup		Common-Operation
 * @brief		設定温度自動復帰タイマー起動
 * @return		戻り値 - なし
 * @note		36MAから移植
 * @warning		なし
 * @date		2016.02.03
 * @author		MSW S.Sugimoto
 */
void autotemp_auto_return_start(void)
{
	/*++設定温度自動復帰タイマーを起動する*/
	struct temp_auto_return	temp_auto_return;
	union temp	temp;					//<[A_017 NSCS_KABASAWA 080610]>
	union temp	temp2;					// 35MA 131203 chata B3コマンド対応・設定温度範囲拡張
	struct settemp_as settemp_as;		// 35MA 131203 chata B3コマンド対応・設定温度範囲拡張
	struct ic_func_limit	func;		// 35MA 131203 chata B3コマンド対応・設定温度範囲拡張
	UB	mode;
	UB	power;
	UB alarm_id;
	UW time;
	UB quickmode_ac;

	/*++設定温度自動復帰取得する*/
	read_db_temp_auto_return_fc(&temp_auto_return);
	/*--*/

// 35MA 131203 chata B3コマンド対応・設定温度範囲拡張 start
	//apl_gettemp(&temp);
	/*++運転モード、室内機機能取得する*/
	apl_get_ac2apl_convert_mode_fc(&mode);
	apl_get_ac2apl_convert_ic_func_limit_fc(&func);
	/*--*/
	/*++ 設定温度を取得する */
	if(DB_DISABLE == func.mode_exp_mc){
		/* 拡張対応無 */
		apl_get_ac2apl_convert_temp_fc(&temp);
	}else{
		/* 拡張対応有 */
		apl_get_ac2apl_convert_settemp_as_fc(&settemp_as);
		switch(mode){
			case UMODE_COOL:			/*	冷房		*/
			case UMODE_DRY:				/*	ドライ		*/
				temp = settemp_as.cool_mx;
				temp2 = settemp_as.heat_mx;	/* 使用しないので初期化する必要性はないが、QAC対応として初期化する	*/
				break;
			case UMODE_HEAT:
			case UMODE_BURNHEAT:
				temp = settemp_as.heat_mx;
				temp2 = settemp_as.heat_mx;	/* 使用しないので初期化する必要性はないが、QAC対応として初期化する	*/
				break;
			case UMODE_AUTOCOOL:		//	自動冷房					
			case UMODE_AUTOHEAT:		//	自動暖房					
				// 拡張対応時の自動冷房/暖房は自動の設定温度と比較する	
				temp = settemp_as.auto_mx;									
				temp2 = settemp_as.heat_mx;	/* 使用しないので初期化する必要性はないが、QAC対応として初期化する	*/
				break;							
			case UMODE_AUTO_2:					
				temp = settemp_as.cool_mx;		
				temp2 = settemp_as.heat_mx;	
				break;
			default:
				temp = settemp_as.cool_mx;
				temp2 = settemp_as.heat_mx;	/* 使用しないので初期化する必要性はないが、QAC対応として初期化する	*/
				break;
		}
	}
	/*--*/
// 35MA 131203 chata  B3コマンド対応・設定温度範囲拡張 end
	/*++ 発停情報を取得する */
	apl_get_ac2apl_convert_power_fc(&power);
	/*--*/
	/*++時短モードを取得する*/
	read_db_quickmode_fc(&quickmode_ac);
	/*--*/
	/*++設定温度自動復帰＝有効の場合のみ処理を実施する */
	if(temp_auto_return.enable == DB_ENABLE) {					//[A_031 NS_KOYAMA 080809]				
	
		switch(mode){				/*	運転モード＝？*/
		case UMODE_COOL:			/*	冷房		*/
		case UMODE_AUTOCOOL:		/*	自動冷房	*/			//[A_017 NS_KOYAMA 080611]
		case UMODE_DRY:				/*	ドライ		*/			//[A_017 NS_KOYAMA 080611]
			/* 室温設定≠無効値 && 自動復帰時間≠無効値?[A_023 honda 080718] */
			if((TEMP_UNDEFINED != temp_auto_return.cooltemp.c.integer) && (UNDEFINED != temp_auto_return.cooltime)){	/*[A_023 honda 080718] */
				/*	室温設定≠自動復帰冷房温度？*/				//[A_021 NS_KOYAMA 080705]
				if(temp.f != temp_auto_return.cooltemp.f){		//[A_021 NS_KOYAMA 080705]	
					/*	アラームID←設定温度自動復帰ＩＤ	*/
					alarm_id = ALARMID_TEMP_AUTO_RETURN;
					/*	アラーム時間(ms)←設定温度自動復帰時間(min)	*/
					if(DB_DISABLE != quickmode_ac){
						/* 時短モード */
						time = (UW)temp_auto_return.cooltime * 10;	// 1分=1秒に変換
					}else{
						/* 通常処理 */
						time = (UW)temp_auto_return.cooltime * 60 * 10;
					}
					/*	アラーム起動要求（alarm_start）	*/
					alarm_start(alarm_id, time,OP_NONE);
				}
			}
			break;
		case UMODE_HEAT:			/*	暖房		*/		//[A_017 NS_KOYAMA 080611]
		case UMODE_AUTOHEAT:		/*	自動暖房	*/		//[A_017 NS_KOYAMA 080611]
		case UMODE_BURNHEAT:		/*	燃焼暖房	*/		//[A_034 NS_KOYAMA 080823]
			/* 室温設定≠無効値 && 自動復帰時間≠無効値?[A_023 honda 080718] */
			if((TEMP_UNDEFINED != temp_auto_return.heattemp.c.integer) && (UNDEFINED != temp_auto_return.heattime)){	/*[A_031 honda 080809] *///冷房ではなく暖房
				/*	室温設定≠自動復帰暖房温度？*/				//[A_021 NS_KOYAMA 080705]
				if(temp.f != temp_auto_return.heattemp.f){		//[A_021 NS_KOYAMA 080705]
					/*	アラームID←設定温度自動復帰ＩＤ	*/
					alarm_id = ALARMID_TEMP_AUTO_RETURN;
					/*	アラーム時間(ms)←設定温度自動復帰（暖房時間）(min)	*/
					if(DB_DISABLE != quickmode_ac){
						/* 時短モード */
						time = (UW)temp_auto_return.heattime * 10;	// 1分=1秒に変換
					}else{
						/* 通常処理 */
						time = (UW)temp_auto_return.heattime * 60 * 10;
					}
					/*	アラーム起動要求（alarm_start）	*/
					alarm_start(alarm_id, time,OP_NONE);
				}
			}
			break;
// 35MA 131203 chata B3コマンド対応・設定温度範囲拡張 strat
		case UMODE_AUTO_2:		/*	自動二値	*/
			/* 室温設定≠無効値 && 自動復帰時間≠無効値? */
			if((TEMP_UNDEFINED != temp_auto_return.heattemp.c.integer) && (UNDEFINED != temp_auto_return.heattime)){
				/*	室温設定≠自動復帰暖房温度？*/
				if(temp2.f != temp_auto_return.heattemp.f){
					/*	アラームID←設定温度自動復帰ＩＤ	*/
					alarm_id = ALARMID_TEMP_AUTO_RETURN2;	//自動二値暖房用に別タイマー
					/*	アラーム時間(ms)←設定温度自動復帰（暖房時間）(min)	*/
					if(DB_DISABLE != quickmode_ac){
						/* 時短モード */
						time = (UW)temp_auto_return.heattime * 10;	// 1分=1秒に変換
					}else{
						/* 通常処理 */
						time = (UW)temp_auto_return.heattime * 60 * 10;
					}
					/*	アラーム起動要求（alarm_start）	*/
					alarm_start(alarm_id, time,OP_NONE);
				}
			}
			/* 室温設定≠無効値 && 自動復帰時間≠無効値? */
			if((TEMP_UNDEFINED != temp_auto_return.cooltemp.c.integer) && (UNDEFINED != temp_auto_return.cooltime)){
				/*	室温設定≠自動復帰冷房温度？*/
				if(temp.f != temp_auto_return.cooltemp.f){
					/*	アラームID←設定温度自動復帰ＩＤ	*/
					alarm_id = ALARMID_TEMP_AUTO_RETURN;
					/*	アラーム時間(ms)←設定温度自動復帰時間(min)	*/
					if(DB_DISABLE != quickmode_ac){
						/* 時短モード */
						time = (UW)temp_auto_return.cooltime * 10;	// 1分=1秒に変換
					}else{
						/* 通常処理 */
						time = (UW)temp_auto_return.cooltime * 60 * 10;
					}
					/*	アラーム起動要求（alarm_start）	*/
					alarm_start(alarm_id, time,OP_NONE);
				}
			}
			break;
// 35MA 131203 chata B3コマンド対応・設定温度範囲拡張 end
		default:
			break;
		}
	}
	/*--*/
}

/****************************************************************************
 *	消し忘れ防止タイマー＜080428追加＞
 ****************************************************************************/
/*****************************************************************
 *	消し忘れ防止タイマー起動
 *	パラメータ：なし
 *	リターン  ：なし
 *	機能：	下記の時、ＤＢの消し忘れ防止時間をアラームに設定（起動）する。
 *			・ＤＢの消し忘れ防止タイマーが有効
 *			・ＤＢの運転/停止が運転
 *****************************************************************/
/**
 * @ingroup		Common-Operation
 * @brief		消し忘れ防止タイマー起動
 * @return		戻り値 - なし
 * @note		36MAから移植
 * @warning		なし
 * @date		2016.02.03
 * @author		MSW S.Sugimoto
 */
void autooff_timer_start(void)
{
	/*++消し忘れ防止タイマーを起動する*/
	struct autooff_timer autooff_timer;
	UB power;
	struct vent vent;							//<[A_033 NS_KOYAMA 080818]>
	UB alarm_id;
	UW time;
	UB quickmode_ac;

	/*++消し忘れ防止タイマー、発停、換気取得*/
	read_db_autooff_timer_fc(&autooff_timer);
	apl_get_ac2apl_convert_power_fc(&power);
	apl_get_ac2apl_convert_vent_fc(&vent);		//<[A_033 NS_KOYAMA 080818]>
	/*--*/
	/*++時短モードを取得する*/
	read_db_quickmode_fc(&quickmode_ac);
	/*--*/
	/*	消し忘れ防止タイマー有効／無効＝有効 and 運転/停止＝運転？	*/
	if((autooff_timer.enable == DB_ENABLE)
	&& ((power == POWER_ON) || (vent.power == VENTPOWER_ON))){		//<[A_033 NS_KOYAMA 080818]>換気単独中も起動する
		/* 消し忘れ防止時間≠無効値? */								/*[A_023 honda 080718] */
		if(UNDEFINED != autooff_timer.min){							/*[A_023 honda 080718] */
			/*	アラームID←消し忘れ防止タイマーＩＤ	*/
			alarm_id = ALARMID_AUTOOFF_TIMER;
			/*	アラーム時間(ms)←消し忘れ防止タイマー時間(min)	*/
			if(DB_DISABLE != quickmode_ac){
				/* 時短モード */
				time = (UW)autooff_timer.min * 10;	// 1分=1秒に変換
			}else{
				/* 通常処理 */
				time = (UW)autooff_timer.min * 60 * 10;
			}
			/*	アラーム起動要求（alarm_start）	*/
			alarm_start(alarm_id, time,OP_NONE);
		}
	}
	/*--*/
}

/**
 * @ingroup		Common-Operation
 * @brief		設定温度０．５℃補正
 * @return		戻り値 - 補正有無
 * @note		36MAから移植
 * @warning		なし
 * @date		2016.01.06
 * @author		Reiden Tomomatsu
 */
UB apl_temp05_check(void)												//<[A_019 NS_KOYAMA 080625]>
{																		//<[A_019 NS_KOYAMA 080625]>
	/*++設定温度0.5℃補正を実行する*/
	UB						modify = APL_FALSE;							//<[A_019 NS_KOYAMA 080625]>
	union temp				temp;										//<[A_019 NS_KOYAMA 080625]>
	struct rc_disp 			rc_disp;									//<[A_019 NS_KOYAMA 080625]>
	struct ic_func_limit 	ic_func_limit;								//<[A_019 NS_KOYAMA 080625]>

	/*++設定温度、動作モード取得*/
	apl_get_ac2apl_convert_temp_fc(&temp);								//<[A_019 NS_KOYAMA 080625]>
	read_db_rcdisp_fc(&rc_disp);										//<[A_019 NS_KOYAMA 080625]>
	/*--*/
	/* 温度表示1℃ */
	if(rc_disp.temp_cf == TEMP_C) {										//<[A_019 NS_KOYAMA 080625]>
		if(temp.c.point5 == 1) {										//<[A_019 NS_KOYAMA 080625]>
			temp.c.point5 = 0;											//<[A_019 NS_KOYAMA 080625]>
			apl_set_ac2apl_convert_temp_fc(&temp);						//<[A_019 NS_KOYAMA 080625]>
			modify = APL_TRUE;											//<[A_019 NS_KOYAMA 080625]>
		}																//<[A_019 NS_KOYAMA 080625]>
	} else {															//<[A_019 NS_KOYAMA 080625]>
		/* 室内機機能取得 */
		apl_get_ac2apl_convert_ic_func_limit_fc(&ic_func_limit);		//<[A_019 NS_KOYAMA 080625]>
		if(ic_func_limit.temppoint5 == DB_DISABLE) {					//<[A_019 NS_KOYAMA 080625]>
			if(temp.c.point5 == 1) {									//<[A_019 NS_KOYAMA 080625]>
				temp.c.point5 = 0;										//<[A_019 NS_KOYAMA 080625]>
				apl_set_ac2apl_convert_temp_fc(&temp);					//<[A_019 NS_KOYAMA 080625]>
				modify = APL_TRUE;										//<[A_019 NS_KOYAMA 080625]>
			}															//<[A_019 NS_KOYAMA 080625]>
		}																//<[A_019 NS_KOYAMA 080625]>
	}																	//<[A_019 NS_KOYAMA 080625]>
	/*--*/
	/*++設定温度補正有無を返却する*/
	return(modify);														//<[A_019 NS_KOYAMA 080625]>
	/*--*/
}

/**
 * @ingroup		Common-Operation
 * @brief		設定温度０．５℃切捨て
 * @return		戻り値 - 補正の有無
 * @note		自動復帰温度・温度制限・週間スケジュール温度の０．５℃を切り捨てる。36MAから移植
 * @warning		なし
 * @date		2016.01.06
 * @author		Reiden Tomomatsu
 */
UB apl_temp05_check_as_fc(void)
{
	/*++設定温度0.5℃切捨てを実行する*/
	UB						modify = APL_FALSE;
//	union temp				temp;
	struct rc_disp 			rc_disp;
	struct ic_func_limit 	ic_func_limit;
	struct settemp_as settemp_as_ax;			//設定温度(拡張対応)

	UB modify_heat = APL_FALSE;
	UB modify_cool = APL_FALSE;
	UB modify_auto = APL_FALSE;
	UB modify_upper = APL_FALSE;
	UB modify_lower = APL_FALSE;
	
	/*++運転モード、室内機機能、設定温度取得*/
	read_db_rcdisp_fc(&rc_disp);
	apl_get_ac2apl_convert_ic_func_limit_fc(&ic_func_limit);
	apl_get_ac2apl_convert_settemp_as_fc(&settemp_as_ax);		/*モード毎設定温度取得 */
	/*--*/
	
	/*++モード毎の設定温度をそれぞれ補正する*/
	modify_heat = apl_temp05_down( &rc_disp, &ic_func_limit, &(settemp_as_ax.heat_mx) );	/* 暖房モード設定温度 */
	modify_cool = apl_temp05_down( &rc_disp, &ic_func_limit, &(settemp_as_ax.cool_mx) );	/* 冷房モード設定温度 */
	modify_auto = apl_temp05_down( &rc_disp, &ic_func_limit, &(settemp_as_ax.auto_mx) );	/* 自動1値モード設定温度 */
	modify_upper = apl_temp05_down( &rc_disp, &ic_func_limit, &(settemp_as_ax.upper_mx) );	/* セットバック上限設定温度 */
	modify_lower = apl_temp05_down( &rc_disp, &ic_func_limit, &(settemp_as_ax.lower_mx) );	/* セットバック下限設定温度 */
	/*--*/
	
	/*++0.5補正されたかどうかの判定 */
	if((APL_TRUE == modify_heat)
	|| (APL_TRUE == modify_cool)
	|| (APL_TRUE == modify_auto)
	|| (APL_TRUE == modify_upper)
	|| (APL_TRUE == modify_lower)){
		/* いずれかで補正発生 */
		modify = APL_TRUE;					/* 補正で返す */
		apl_set_ac2apl_convert_settemp_as_fc(&settemp_as_ax);
	}
	/*--*/

	//従来動作（切捨てる）に戻すため削除を復活させる
	
	//室内温度は追加のまま
	/*--*/
	/*++設定温度補正有無を返却する*/
	return(modify);
	/*--*/
}

/**
 * @ingroup		Common-Operation
 * @brief		設定温度０．５℃切捨て
 * @param		なし
 * @return		戻り値 - 補正の有無
 * @note		自動復帰温度・温度制限・週間スケジュール温度の０．５℃を切り捨てる。36MAから移植
 * @warning		なし
 * @date		2016.01.06
 * @author		Reiden Tomomatsu
 */
void apl_temp05cut(void)								//<[A_033 NS_KOYAMA 080821]>
{
	/*++設定温度０．５℃切り捨てを実行する*/
	UB						week;							//<[A_033 NS_KOYAMA 080821]>
	UB						n;								//<[A_033 NS_KOYAMA 080821]>
	struct temp_auto_return	temp_auto_return;				//<[A_033 NS_KOYAMA 080821]>
//	struct weekly_schedule	weekly_schedule;				//<[A_033 NS_KOYAMA 080821]>
	struct common_weekly_schedule	weekly_schedule;		//<[A_033 NS_KOYAMA 080821]>
	struct temp_limit 	temp_limit;							// [A_033 NS_KOYAMA 080821]
	struct ic_func_limit 	ic_func_limit;					//[V02.01 NS_KOYAMA 081023]
	struct rc_disp			rc_disp;						//[V02.01 NS_KOYAMA 081023]
	UB						fah2f = 0;						//[V02.01 NS_KOYAMA 081023]
	UB						select_ac;						// 36MA 2014/08/26 ogawa 週間スケジュール2枚化
#if defined(ROM_MAA) || defined(ROM_MAAC)
	struct night_setback_setting night_setback_setting_ax;
#endif	// ROM_MAA or ROM_MAAC
	
	/*++必要な情報を取得する*/
	apl_get_ac2apl_convert_ic_func_limit_fc(&ic_func_limit);		//[V02.01 NS_KOYAMA 081023]
	read_db_rcdisp_fc(&rc_disp);							//[V02.01 NS_KOYAMA 081023]
	/*--*/

	/*++*/
	if((ic_func_limit.temppoint5 == 0)						//[V02.01 NS_KOYAMA 081023]
	&& (rc_disp.temp_cf == TEMP_F)) {						//[V02.01 NS_KOYAMA 081023]
		/* ２ﾟF表示 */										//[V02.01 NS_KOYAMA 081023]
		fah2f = 1;											//[V02.01 NS_KOYAMA 081023]
	}														//[V02.01 NS_KOYAMA 081023]
	/*--*/

	/*++ 設定温度自動復帰を更新する*/								// [A_033 NS_KOYAMA 080821]
	read_db_temp_auto_return_fc(&temp_auto_return);			//<[A_033 NS_KOYAMA 080821]>
	
	if((fah2f == 1)											//[V02.01 NS_KOYAMA 081023]
	&& (temp_auto_return.cooltemp.c.integer == 20)) {		//[V02.01 NS_KOYAMA 081023]
		temp_auto_return.cooltemp.c.integer = 19;			//[V02.01 NS_KOYAMA 081023]
	}														//[V02.01 NS_KOYAMA 081023]
	
	if((fah2f == 1)											//[V02.01 NS_KOYAMA 081023]
	&& (temp_auto_return.heattemp.c.integer == 20)) {		//[V02.01 NS_KOYAMA 081023]
		temp_auto_return.heattemp.c.integer = 19;			//[V02.01 NS_KOYAMA 081023]
	}														//[V02.01 NS_KOYAMA 081023]
	
	temp_auto_return.cooltemp.c.point5 = 0;					//<[A_033 NS_KOYAMA 080821]>自動復帰温度の0.5℃クリア
	temp_auto_return.heattemp.c.point5 = 0;					//<[A_033 NS_KOYAMA 080821]>
	
	write_db_temp_auto_return_fc(&temp_auto_return);		//<[A_033 NS_KOYAMA 080821]>
	updateinfo.temp_auto_return_ege = 1;					//<[A_033 NS_KOYAMA 080821]>
	/*--*/
															// [A_033 NS_KOYAMA 080821]
	/*++ 週間スケジュール設定情報を更新する*/								// [A_033 NS_KOYAMA 080821]
	/* 週間スケジュール温度切捨て*/							// [A_033 NS_KOYAMA 080821]
	for( select_ac = 0 ; select_ac < SCHEDULE_NUM_MAX ; select_ac++){	// 36MA 2014/08/26 ogawa 週間スケジュール2枚化
		for(week = 0; week != WEEK; week++) {					//<[A_033 NS_KOYAMA 080821]>
//			read_db_weekly_schedule(week, &weekly_schedule);	//<[A_033 NS_KOYAMA 080821]>
//			read_db_weekly_schedule(select_ac,week, &weekly_schedule);	// 36MA 2014/08/26 ogawa 週間スケジュール2枚化
			read_db_aweek_schedule_fc(select_ac,week,&weekly_schedule);	// 36MA 2014/09/08 ogawa 


			for(n = 0; n != WEEKLYTIMER_MAX; n++) {				//<[A_033 NS_KOYAMA 080821]>
				if((fah2f == 1)												//[V02.01 NS_KOYAMA 081023]
//				   && (weekly_schedule.schedule[n].temp.c.integer == 20)) {	//[V02.01 NS_KOYAMA 081023]
//					weekly_schedule.schedule[n].temp.c.integer = 19;		//[V02.01 NS_KOYAMA 081023]
				   && (weekly_schedule.schedule_mx[n].temp_mx.c.integer == 20)) {		  // 36MA 2014/09/08 ogawa
					weekly_schedule.schedule_mx[n].temp_mx.c.integer = 19;				  // 36MA 2014/09/08 ogawa
				}																		  // 36MA 2014/09/08 ogawa
				
			if((fah2f == 1)												//35MA 131203 chata  B3コマンド対応・自動二値
//			   && (weekly_schedule.schedule[n].temp2_mx.c.integer == 20)) {	//35MA 131203 chata  B3コマンド対応・自動二値
//				weekly_schedule.schedule[n].temp2_mx.c.integer = 19;		//35MA 131203 chata  B3コマンド対応・自動二値
			   && (weekly_schedule.schedule_mx[n].temp2_mx.c.integer == 20)) {	/* B3コマンド対応・自動二値 */  // 36MA 2014/09/08 ogawa
				weekly_schedule.schedule_mx[n].temp2_mx.c.integer = 19;			/* B3コマンド対応・自動二値 */  // 36MA 2014/09/08 ogawa
			}																	/* B3コマンド対応・自動二値 */  // 36MA 2014/09/08 ogawa
				
//			weekly_schedule.schedule[n].temp.c.point5 = 0;	//<[A_033 NS_KOYAMA 080821]>スケジュール温度の0.5℃クリア
//			weekly_schedule.schedule[n].temp2_mx.c.point5 = 0;				//35MA 131203 chata  B3コマンド対応・自動二値
			weekly_schedule.schedule_mx[n].temp_mx.c.point5 = 0;	/* スケジュール温度の0.5℃クリア*/	// 36MA 2014/09/08 ogawa
			weekly_schedule.schedule_mx[n].temp2_mx.c.point5 = 0;	/* B3コマンド対応・自動二値		*/  // 36MA 2014/09/08 ogawa
			
			}													//<[A_033 NS_KOYAMA 080821]>
//		write_db_weekly_schedule(week, &weekly_schedule);	//<[A_033 NS_KOYAMA 080821]>
//			write_db_weekly_schedule(select_ac , week, &weekly_schedule);	// 36MA 2014/08/26 ogawa 週間スケジュール2枚化
			write_db_aweek_schedule_fc(select_ac , week, &weekly_schedule);	// 36MA 2014/09/08 ogawa
		}														//<[A_033 NS_KOYAMA 080821]>
	}
															// [A_033 NS_KOYAMA 080821]
	/*++設定温度範囲制限を更新する */						//<[A_033 NS_KOYAMA 080821]>
	/* 温度範囲制限切捨て */								//<[A_033 NS_KOYAMA 080821]>
	read_db_temp_limit_fc(&temp_limit);						// [A_033 NS_KOYAMA 080821]
															// [A_033 NS_KOYAMA 080821]
	if(fah2f == 1) {										//[V02.01 NS_KOYAMA 081023]
		if(temp_limit.cool_upper.c.integer == 20) {			//[V02.01 NS_KOYAMA 081023]
			temp_limit.cool_upper.c.integer = 19;			//[V02.01 NS_KOYAMA 081023]
		}													//[V02.01 NS_KOYAMA 081023]
		if(temp_limit.cool_lower.c.integer == 20) {			//[V02.01 NS_KOYAMA 081023]
			temp_limit.cool_lower.c.integer = 19;			//[V02.01 NS_KOYAMA 081023]
		}													//[V02.01 NS_KOYAMA 081023]
															//[V02.01 NS_KOYAMA 081023]	
		if(temp_limit.heat_upper.c.integer == 20) {			//[V02.01 NS_KOYAMA 081023]
			temp_limit.heat_upper.c.integer = 19;			//[V02.01 NS_KOYAMA 081023]
		}													//[V02.01 NS_KOYAMA 081023]
		if(temp_limit.heat_lower.c.integer == 20) {			//[V02.01 NS_KOYAMA 081023]
			temp_limit.heat_lower.c.integer = 19;			//[V02.01 NS_KOYAMA 081023]
		}													//[V02.01 NS_KOYAMA 081023]
															//[V02.01 NS_KOYAMA 081023]
		if(temp_limit.auto_upper.c.integer == 20) {			//[V02.01 NS_KOYAMA 081023]
			temp_limit.auto_upper.c.integer = 19;			//[V02.01 NS_KOYAMA 081023]
		}													//[V02.01 NS_KOYAMA 081023]
		if(temp_limit.auto_lower.c.integer == 20) {			//[V02.01 NS_KOYAMA 081023]
			temp_limit.auto_lower.c.integer = 19;			//[V02.01 NS_KOYAMA 081023]
		}													//[V02.01 NS_KOYAMA 081023]
	}														//[V02.01 NS_KOYAMA 081023]

	if(DB_DISABLE == ic_func_limit.mode_exp_mc) {												// 35MA 131203 chata  B3コマンド対応・設定温度範囲拡 範囲制限0.5℃補正
		temp_limit.cool_upper.c.point5 = 0;						// [A_033 NS_KOYAMA 080821]
		temp_limit.cool_lower.c.point5 = 0;						// [A_033 NS_KOYAMA 080821]

		temp_limit.heat_upper.c.point5 = 0;						// [A_033 NS_KOYAMA 080821]
		temp_limit.heat_lower.c.point5 = 0;						// [A_033 NS_KOYAMA 080821]
																// [A_033 NS_KOYAMA 080821]
		temp_limit.auto_upper.c.point5 = 0;						// [A_033 NS_KOYAMA 080821]
		temp_limit.auto_lower.c.point5 = 0;						// [A_033 NS_KOYAMA 080821]
		
	// 35MA 131203 chata  B3コマンド対応・設定温度範囲拡 範囲制限0.5℃補正 start
	} else {	// モード・設定温度範囲拡張対応時
		//冷房制限範囲下限
		if ( 1 == temp_limit.cool_lower.c.point5 ){
			temp_limit.cool_lower.c.integer = temp_limit.cool_lower.c.integer +1;				// 切り上げ
			temp_limit.cool_lower.c.point5 = 0;
		}
		
		//冷房制限範囲上限
		if ( 1 == temp_limit.cool_upper.c.point5 ){
			temp_limit.cool_upper.c.point5 = 0;													// 切り下げ
		}
		
		//暖房制限範囲下限
		if ( 1 == temp_limit.heat_lower.c.point5 ){
			temp_limit.heat_lower.c.integer = temp_limit.heat_lower.c.integer +1;				// 切り上げ
			temp_limit.heat_lower.c.point5 = 0;
		}
		
		//暖房制限範囲上限
		if ( 1 == temp_limit.heat_upper.c.point5 ){
			temp_limit.heat_upper.c.point5 = 0;													// 切り下げ
		}
		
		//自動制限範囲下限
		if ( 1 == temp_limit.auto_lower.c.point5 ){
			temp_limit.auto_lower.c.integer = temp_limit.auto_lower.c.integer +1;				// 切り上げ
			temp_limit.auto_lower.c.point5 = 0;
		}
		
		//自動制限範囲上限
		if ( 1 == temp_limit.auto_upper.c.point5 ){
			temp_limit.auto_upper.c.point5 = 0;													// 切り下げ
		}
		
	}
	// 35MA 131203 chata  B3コマンド対応・設定温度範囲拡 範囲制限0.5℃補正 end
	/*--*/
															// [A_033 NS_KOYAMA 080821]
	write_db_temp_limit_fc(&temp_limit);						// [A_033 NS_KOYAMA 080821]									// [A_022 NS_KOYAMA 080709]
	apl_set_ac2apl_convert_temp_limit_fc(&temp_limit);
#if defined(ROM_MAA) || defined(ROM_MAAC)
	/*++ナイトセットバック設定温度 (上限値/下限値) 切り捨て*/
	read_db_night_setback_setting_fc(&night_setback_setting_ax);	// DB値取得
	night_setback_setting_ax.temp_upper.c.point5 = DB_DISABLE;		// 上限値 0.5℃切り捨て
	night_setback_setting_ax.temp_lower.c.point5 = DB_DISABLE;		// 下限値 0.5℃切り捨て
	write_db_night_setback_setting_fc(&night_setback_setting_ax);	// DB値更新
	/*--*/
#endif	// ROM_MAA or ROM_MAAC
	/*--*/
}

/**
 * @ingroup		Common-Operation
 * @brief		温度０．５℃補正
 * @param[in]		*p_disp
 * @param[in]		*p_limit
 * @param[in]		*p_temp  
 * @return		戻り値 - 補正有無
 * @note		36MAから移植
 * @warning		なし
 * @date		2016.01.06
 * @author		Reiden Tomomatsu
 */
UB apl_temp05_down(const struct rc_disp* p_disp,const struct ic_func_limit* p_limit,union temp* p_temp)
{
	/*++温度0.5℃補正を実行する*/
	UB modify = APL_FALSE;
	
	/*++温度1℃表示で小数部が有効なとき、切り捨てを行う*/
	/* リモコン表示設定が、1℃表示の場合 */
	if(TEMP_C == p_disp->temp_cf) {
		/* 小数部が有効の場合 */
		if(1 == p_temp->c.point5) {
			/* 切り捨てる */
			p_temp->c.point5 = 0;
			modify = APL_TRUE;
		}
	}
	/*--*/
	else {
		/*++温度1℃表示以外で温度表示単位(0.5℃表示)が有効なとき、切り捨てを行う*/
		/*温度表示単位(0.5℃表示)が有効 */
		if(DB_DISABLE == p_limit->temppoint5) {
			/* 小数部が有効の場合 */
			if(1 == p_temp->c.point5) {
				/* 切り捨てる */
				p_temp->c.point5 = 0;
				modify = APL_TRUE;
			}
			
		}
	}
	/*--*/
	/*--*/
	
	/*++温度補正有無を返却する*/
	return(modify);
	/*--*/
}

/**
 * @ingroup		Common-Operation
 * @brief		異常リセット
 * @return		なし
 * @note		36MAから移植
 * @warning		なし
 * @date		2016.01.06
 * @author		Reiden Tomomatsu
 */
void aplutl_errorreset(void)									//<[A_020 NS_KOYAMA 080627]>
{																//<[A_020 NS_KOYAMA 080627]>
	/*++異常リセットを実行する*/
	struct error	error;										//<[A_020 NS_KOYAMA 080627]>
//	UB				power;										//<[A_020 NS_KOYAMA 080627]>	//[35MA A09 SEC touyou 131227] 未使用Auto変数削除
	//UB				clock_state;							//<[A_029 NS_KOYAMA 080804]>
#ifdef MODEL_MA
	UB	vent_24h ;												//[32MA A06 101026 sugimoto]不具合修正
	UB	masterslave_ac;											/* 42MAAB_R32関連対応_MESW_Inoue_250807 */  /* 47MA-SE_42MAABから移植_MESW_Inoue_20251105 */
#endif

	/*++異常情報を取得する*/
	read_db_error_fc(&error);									//<[A_020 NS_KOYAMA 080626]>
	/*--*/

#ifdef MODEL_MA
	/* 主従設定を取得する */
	read_db_masterslave_fc(&masterslave_ac);					/* 42MAAB_R32関連対応_MESW_Inoue_250807 */  /* 47MA-SE_42MAABから移植_MESW_Inoue_20251105 */
#endif

	/*++異常なし以外の場合、処理を実施する*/
	if(error.err_mente != ERROR_NONE) {							//<[A_020 NS_KOYAMA 080626]>

// 通信異常解除の場合は3分応答なし異常リスタート。
// ただし、
// EC2_NORX が(EC_Ex | EC_x0)
// EC_Ex が0x40
// EC_x0 が0x10
// EC4_NORX が0x6831
// とdefineされている前提。
// 上記コードが変更になった場合は、aplima.c内のdefineに応じて
// 下記の条件式を修正する必要がある。注意！！！

//		{
//			struct error	error_reset;							//<[A_033 NS_KOYAMA 080821]>
//			error_reset = error;									//<[A_033 NS_KOYAMA 080821]>
//			/* 停止中の異常リセット */								//<[A_020 NS_KOYAMA 080626]>						
//			error_reset.err_mente = ERROR_NONE;						//<[A_020 NS_KOYAMA 080627]>
//			write_db_error_fc(&error_reset);						//<[A_020 NS_KOYAMA 080627]>
//			apl_set_ac2apl_convert_error_fc(&error_reset);
//		}															//<[A_020 NS_KOYAMA 080626]>
		{
			UB	work;
			UB	slimfp;													//[A_033 NS_KOYAMA 080821]
			read_db_rcstate_fc(&work);
#ifdef MODEL_MA
			apl_get_ac2apl_convert_slimfpconbi_fc(&slimfp);							//[A_033 NS_KOYAMA 080821]
#else	// MODEL_ME
			/*	MEの場合はマルチ固定	*/
			slimfp = SLIMFPCONBI_FP;
#endif	// MODEL_MA
			if ((work == RCSTATE_RUN) || (work == RCSTATE_TEST)) {	// 立ち上げ中は強制停止/運転しない
				UB	check = APL_FALSE;
				#if 0
				if(slimfp == SLIMFPCONBI_FP) {							//[A_033 NS_KOYAMA 080821]マルチは強制送信可
					// マルチ											//[A_033 NS_KOYAMA 080821]
					check = APL_TRUE;									//[A_033 NS_KOYAMA 080821]
				} else {												//[A_033 NS_KOYAMA 080821]
					// スリム											//[A_033 NS_KOYAMA 080821]
					if(error.err_mente == ERROR_MENTE) {				//[A_033 NS_KOYAMA 080821]
						if((error.codelong != EXCEP_ERR_3604)			//[A_033 NS_KOYAMA 080821]スリムのメンテ3604/3605は強制送信しない
						&& (error.codelong != EXCEP_ERR_3605)) {		//[A_033 NS_KOYAMA 080821]
							check = APL_TRUE;							//[A_033 NS_KOYAMA 080821]
						}												//[A_033 NS_KOYAMA 080821]
					} else {	// == ERROR_ERROR						//[A_033 NS_KOYAMA 080821]
						check = APL_TRUE;								//[A_033 NS_KOYAMA 080821]
					}													//[A_033 NS_KOYAMA 080821]
				}														//[A_033 NS_KOYAMA 080821]
				#else
				if(slimfp == SLIMFPCONBI_SLIM) {
#ifdef MODEL_MA
					// スリム
					if(error.err_mente == ERROR_MENTE) {
						if((error.codelong != EXCEP_ERR_3604)
						&& (error.codelong != EXCEP_ERR_3605)) {
							check = APL_TRUE;
						}
					} else {	// == ERROR_ERROR
						check = APL_TRUE;
					}
#endif
				} else {
					/*ﾏﾙﾁ or 外気*/	//[32MA A05 100907 sugimoto]
					check = APL_TRUE;
				}
				#endif
				if(check == APL_TRUE) {										//[A_033 NS_KOYAMA 080821]
					if( ( SLIMFPCONBI_SLIM == slimfp ) || ( SLIMFPCONBI_FP == slimfp ) ){
						
					} else {											//[32MA A05 100907 sugimoto]
#ifdef MODEL_MA
						//外気											//[32MA A05 100907 sugimoto]
						/* 管理人室リモコンからはコマンド送信しない */
						if (RC_SUPERVISOR != masterslave_ac)			/* 42MAAB_R32関連対応_MESW_Inoue_250807 */  /* 47MA-SE_42MAABから移植_MESW_Inoue_20251105 */
						{
							vent_24h = FU_24H_DRV_OFF;						//[32MA A06 101026 sugimoto]不具合修正
							apl_set_ac2apl_convert_fu_24h_drv_fc(&vent_24h);		/* 2017/5/12 MSW ogawa #6692 水平展開 */				//[32MA A06 101026 sugimoto]不具合修正
							/* 操作設定b2要求	*/							//[32MA A05 100907 sugimoto]
	//						updateinfo.sousa_b2 = 1;						//[32MA A05 100907 sugimoto]
						}												/* 42MAAB_R32関連対応_MESW_Inoue_250807 */  /* 47MA-SE_42MAABから移植_MESW_Inoue_20251105 */
#endif
					}
				}
			}
		}
		
		/*	ＧＵＩ異常解除更新イベント)		*/					//<[A_020 NS_KOYAMA 080626]>
//		snd_msg(ID_event,(PT_MSG*)IMM_EV_APL_ERR_REST);			//<[A_020 NS_KOYAMA 080627]>
		snd_gui_event_msg(IMM_EV_APL_ERR_REST);
	}															//<[A_020 NS_KOYAMA 080627]>
	/*--*/
}
/**
 * @ingroup		Common-Operation
 * @brief		異常リセット
 * @param		- なし
 * @return		戻り値 - なし
 * @note		36MAから移植
 * @warning		なし
 * @date		2016.01.06
 * @author		Reiden Tomomatsu
 */
void aplutl_rtc_errorreset(void)														//<[A_034 NS_KOYAMA 080825]>
{																						//<[A_034 NS_KOYAMA 080825]>
	/*++RTC異常リセットを実行する*/
	struct error	error;																//<[A_034 NS_KOYAMA 080825]>
	UB				clock_state;														//<[A_034 NS_KOYAMA 080825]>
																						//<[A_034 NS_KOYAMA 080825]>
	/*++DBから異常情報を読み込む*/
	read_db_error_fc(&error);															//<[A_034 NS_KOYAMA 080825]>
	/*--*/
	/*++異常情報をチェックする*/
	if((error.err_mente == ERROR_MENTE)													//<[A_034 NS_KOYAMA 080825]>
	&& (error.codelong == RTC_ERR_LONGCODE)) {											//<[A_034 NS_KOYAMA 080825]>
		/*++時計時刻未設定にする*/
		clock_state = CLOCK_INI;														//<[A_034 NS_KOYAMA 080825]>
		write_db_isclockset_fc(&clock_state);											//<[A_034 NS_KOYAMA 080825]>
#ifdef MODEL_MA
		apl_set_ac2apl_convert_clockstate_fc(&clock_state);
#else  /* MODEL_ME */
#endif /* MODEL_MA */
		/*--*/
		/* 時計時刻更新あり */
		updateinfo.clock_chg = 1;														//<[A_034 NS_KOYAMA 080825]>
		/* 不揮発記憶要求あり */														//<[A_034 NS_KOYAMA 080825]>
		updateinfo.eewrreq = 1;															//<[A_034 NS_KOYAMA 080825]>
				
		/*++ble_reset_24timer制御を開始する*/
		aplutl_set_ble_reset_24timer_fv();		///国内版42MA_定期的RL78リセット機能対応_chata_20190826
		/*--*/
		
	}																					//<[A_034 NS_KOYAMA 080825]>
	
	/*--*/
	/*--*/
}

/**
 * @ingroup		Common-Operation
 * @brief		調停コントローラの起動要求
 * @return		なし
 * @note		アプリケーションタスク内部からの実行用関数
 * @warning		
 * @date		2016.2.16
 * @author		MSW S.Sugimoto
 */
void start_arbitration_controller(void)							//<[A_020 NS_KOYAMA 080627]>
{
	/*++アプリケーション内部からの要求により、調停コントローラを起動する*/
	struct apl_buf buffer_ax;
	/*タスクID：アプリケーション内部*/
	buffer_ax.src_mi = SRCTASK_ID_INTERNAL;
	/*動作要求：調停コントローラ*/
	buffer_ax.pattern_mi = PATTERN_SELECT_ARBITRATION;
	/*共通/個別識別：共通部*/
	buffer_ax.type_mi = TYPE_SELECT_COMMON;
	/*++メッセージバッファへの送信を実行する*/
	snd_message_fc(&buffer_ax);
	/*--*/
	/*--*/
}

#ifdef DEBUG_PRINT
/**
 * @ingroup		application
 * @brief		ファイル名テーブルからファイルインデックスを取得
 * @param[in]	* file_name_rcp - 発生ファイル名
 * @param[in]	* result_rcp - ファイル名に対応する数値
 * @return		処理結果
 * @note		
 * @warning		
 * @date		2016.2.23
 * @author		Reiden Tomomatsu
 */
#define MAX_FILE_NAME	48
struct file_name_cnvt{
	const UB filename[MAX_FILE_NAME];
	UB num;
};

const struct file_name_cnvt file_name_cnvt_tbl[]  = {
	{"apl_alarm.c",					0x00},
	{"apl_atlstc.c",				0x01},
	{"apl_callback.c",				0x02},
	{"apl_main.c",					0x03},
	{"apl_utility.c",				0x04},
	{"arbitration_controller.c",	0x05},
	{"apl_db_common.c",				0x10},
	{"apl_if_common.c",				0x11},
	{"apl_notifyif_common.c",		0x12},
	{"apl_direct_common.c",			0x13},
	{"apl_model_common_util.c",		0x14},
	{"apl_notify_common.c",			0x15},
	{"apl_operation_common.c",		0x16},
	{"apl_schedule_common.c",		0x17},
	{"apl_timer_common.c",			0x18},
	{"apl_db_ma.c",					0x10},
	{"apl_if_ma.c",					0x11},
	{"apl_notifyif_ma.c",			0x12},
	{"apl_direct_ma.c",				0x13},
	{"apl_notify_ma.c",				0x14},
	{"apl_operation_ma.c",			0x15},
	{"apl_schedule_ma.c",			0x16},
	{"apl_timer_ma.c",				0x17},
	{"apl_db_me.c",					0x20},
	{"apl_if_me.c",					0x21},
	{"apl_notifyif_me.c",			0x22},
	{"apl_direct_me.c",				0x23},
	{"apl_notify_me.c",				0x24},
	{"apl_operation_me.c",			0x25},
	{"apl_schedule_me.c",			0x26},
	{"apl_timer_me.c",				0x27},
};

static UB file_name_cmpr_fc(UB * file_name_rcp,UB *result_rcp)
{
	/*++ファイル名テーブルからファイルインデックスを取得する*/
	/* DEBUG_PRINT有効時、この関数は有効 */
	UB loop;
	UB tbl_num;
	UB rtn = APL_FALSE;
	
	/*++ファイル名テーブルからファイルインデックスを取得する*/
	/* 見つからない場合は0xffを返す */
	for(tbl_num = 0; tbl_num < sizeof( file_name_cnvt_tbl ) / sizeof( struct file_name_cnvt ); tbl_num++){	//テーブルの縦列
		for(loop = 0; loop < MAX_FILE_NAME; loop++){	/* ファイル名を1文字ずつ比較 */
			if(  file_name_rcp[loop] != '\0' ){
				if(file_name_rcp[loop] != file_name_cnvt_tbl[tbl_num].filename[loop]){	/* ファイル名違ったらbreak */
					rtn = APL_FALSE;	/* ファイル名が異なると0 */
					break;
				}else{
					rtn = APL_TRUE;		/* ファイル名が同じ限り1 */
				}
			} else {
				break;
			}
		}
		if( APL_TRUE == rtn ) {
			break;
		}
	}
	/*--*/
	/*++検索に成功していたら引数のアドレスにファイルインデックスを設定する*/
	if(APL_TRUE == rtn){
		/* ファイル名に対応する数値をrtnに入れる */
		*result_rcp = file_name_cnvt_tbl[tbl_num].num;
	}
	/*--*/
	/*--*/
	/*++実行結果を返却する*/
	return rtn;
	/*--*/
}

/**
 * @ingroup		application
 * @brief		エラー出力系デバッグ情報 例外情報
 * @param[in]	* file_name_rcp - 発生ファイル名
 * @param[in]	line_num_ru - 発生ライン数
 * @param[in]	err_code_rc - 付随情報
 * @return		debuguart_set_b_fcの結果
 * @note		
 * @warning		
 * @date		2016.2.23
 * @author		Reiden Tomomatsu
 */
UB apl_debug_uart_exception_fc(UB * file_name_rcp, UH line_num_ru, UB err_code_rc)
{
	/*++例外情報を出力する*/
	/* DEBUG_PRINT有効時、この関数は有効 */
	UB data[8];
	UB ret,fileindex;
	
	data[0] = 0x80 | 0x02;		/* b7:1 Gemini→PC、b6～b0:0x02アプリケーション */
	data[1] = 0x00;				/* 例外情報(出力内容識別子：0x00) */
	ret = file_name_cmpr_fc(file_name_rcp,&fileindex);
	if( APL_FALSE == ret ) {
		fileindex = 0xff;
	}
	data[2] = fileindex;
	data[3] = (UB)(line_num_ru  >> 8);
	data[4] = (UB)(line_num_ru & 0x00ff);
	data[5] = err_code_rc;
	data[6] = '\r';
	data[7] = '\n';
	/*--*/
	/*++UARTで処理結果を送信する*/
	return debuguart_set_b_fc(data, 8);
	/*--*/
}

/**
 * @ingroup		application
 * @brief		エラー出力系デバッグ情報 アプリケーション実行エンジンエラー終了情報
 * @param[in]	src_rc - 操作元タスク種別
 * @param[in]	pattern_rc - 動作要求モデル
 * @param[in]	type_rc - 個別/共通識別
 * @param[in]	request_id_rc - 要求ID
 * @return		debuguart_set_b_fcの結果
 * @note		
 * @warning		
 * @date		2016.2.23
 * @author		Reiden Tomomatsu
 */
UB apl_debug_uart_engine_fc(UB src_rc, UB pattern_rc, UB type_rc, UH request_id_rc)
{
	/*++アプリケーション実行エンジンエラー終了情報を出力する*/
	/* DEBUG_PRINT有効時、この関数は有効 */
	UB data[8];

	data[0] = 0x80 | 0x02;		/* b7:1 Gemini→PC、b6～b0:0x02アプリケーション */
	data[1] = 0x01; 			/* アプリケーション実行エンジンエラー終了情報(出力内容識別子：0x01) */
	data[2] = (src_rc << 4) | (pattern_rc & 0x0f);
	data[3] = (type_rc << 4) | 0x00;
	data[4] = (UB)(request_id_rc  >> 8);
	data[5] = (UB)(request_id_rc & 0x00ff);
	data[6] = '\r';
	data[7] = '\n';
	/*--*/
	/*++UARTで処理結果を送信する*/
	return debuguart_set_b_fc(data, 8);
	/*--*/
}
#endif	/*	DEBUG_PRINT	*/

/**
 * @ingroup		application
 * @brief		GUIへイベントを発行する
 * @param[in]	event - 送信イベント
 * @note		
 * @warning		psndを使用している為、データキューがあふれた場合はメッセージが送信されない
 * @date		2016.04.22
 * @author		SEC S.Sugimoto
 */
void snd_gui_event_msg(UH event)
{
	T_Event ev;
	/*++ GUIへイベントを送信する	*/
	
	/*++ 送信データ組み立て	*/
	ev.data = 0x0000;
	ev.com.type = EVENTTYPE_APP;
	ev.com.event = event;
	/*--*/
	
	/*++ 送信する	*/
	/* ※psndを使用している為、データキューがあふれた場合はメッセージが送信されない	*/
	psnd_dtq(ID_event,(VP_INT)ev.data);
	/*--*/
}

/**
 * @ingroup		application
 * @brief		華氏時温度差の取得
 * @param[in]	fugou - 最新値
 * @param[in]	nowtemp - 最新値
 * @param[in]	*para_rcp - 最新値
 * @param[in]	temp05_rc - 最新値
 * @note		
 * @warning		
 * @date		2015.12.21
 * @author		MSW S.Sugimoto
 */
void apl_get_temp_diff_f_fv(UB fugou, union temp nowtemp, union temp *para_rcp, UB temp05_rc)
{
	/*++華氏時温度差を取得する*/
	// 符号（fugou）： 0=マイナス、1=プラス
//	UB i;																		//<20120924_watanabe_冷.79_華氏時の連動結果異常>
	H i;																		//テーブルの先頭までループさせるために符号付きに変更 <20120924_watanabe_冷.79_華氏時の連動結果異常>
	UB now = 0, diff = 0, sum = 0;												// [*:kaishima20121008]QAC結果より余分な変数宣言(ret)削除
//	UB check_ac = 0;	//[35MA A09 SEC touyou 131227] 未使用Auto変数削除
	union temp aftertemp;
	UB change_flag;
	UB temp_min = temp_tbl_ech[0][0];											//最小値 <20120924_watanabe_冷.79_華氏時の連動結果異常>
	UB temp_max = temp_tbl_ech[TEMP_TBL_MAX - 1][1];							//最大値 <20120924_watanabe_冷.79_華氏時の連動結果異常>

	if(nowtemp.c.integer >= TEMP_TBL_MAX){
		diff = 0;
	}else{
		now = temp_tbl_ech[nowtemp.c.integer][nowtemp.c.point5];
		if(temp05_rc == DB_DISABLE && now == 68){	// 0.5℃未対応 & 華氏温度=68ﾟF の場合
			now = 67;							// 表示温度=67ﾟFとする(2ﾟF単位での表示のため)
		}
		diff = para_rcp->c.integer * 2;
		diff += (para_rcp->c.point5 ? 1 : 0);
	}

	if(0 == fugou){
		// マイナス
		sum = now - diff;
//		if(sum > 32){//  0(小数なし)： 0.0℃   32ﾟF								//<20120924_watanabe_冷.79_華氏時の連動結果異常>
		if(sum >= temp_min){//  0(小数なし)： 0.0℃   32ﾟF						//最小値を含むように修正 <20120924_watanabe_冷.79_華氏時の連動結果異常>
			change_flag = apl_change_temp_f_from_c(sum,&aftertemp);				//<20120924_watanabe_冷.79_華氏時の連動結果異常>
			if(change_flag!=APL_TRUE)												//<20120924_watanabe_冷.79_華氏時の連動結果異常>
			{																	//<20120924_watanabe_冷.79_華氏時の連動結果異常>
				/* テーブルから検索する */
//				for(i = nowtemp.c.integer; i > 0; i--){
				for(i = nowtemp.c.integer; i >= 0; i--){						//<20120924_watanabe_冷.79_華氏時の連動結果異常>
					if(sum == temp_tbl_ech[i][0]){
						aftertemp.c.integer = i;
						aftertemp.c.point5 = 0;
						break;
					}
					if(sum == temp_tbl_ech[i][1]){
						aftertemp.c.integer = i;
						aftertemp.c.point5 = 1;
						break;
					}
				}
//				para_rcp->f = (nowtemp.f - aftertemp.f);						//算出箇所を移動 <20120924_watanabe_冷.79_華氏時の連動結果異常>
			}
			para_rcp->f = (nowtemp.f - aftertemp.f);							//<20120924_watanabe_冷.79_華氏時の連動結果異常>
		}
	}
	else{
		// プラス
		sum = now + diff;
//		if(sum < 105){// 39(小数あり)：40.5℃  105ﾟF							//<20120924_watanabe_冷.79_華氏時の連動結果異常>
		if(sum <= temp_max){// 39(小数あり)：40.5℃  105ﾟF						//最大値を含むように修正 <20120924_watanabe_冷.79_華氏時の連動結果異常>
			change_flag = apl_change_temp_f_from_c(sum,&aftertemp);				//<20120924_watanabe_冷.79_華氏時の連動結果異常>
			if(change_flag!=APL_TRUE)												//<20120924_watanabe_冷.79_華氏時の連動結果異常>
			{																	//<20120924_watanabe_冷.79_華氏時の連動結果異常>
				/* テーブルから検索する */
				for(i = nowtemp.c.integer; i < TEMP_TBL_MAX; i++){
					if(sum == temp_tbl_ech[i][0]){
						aftertemp.c.integer = i;
						aftertemp.c.point5 = 0;
						break;
					}
					if(sum == temp_tbl_ech[i][1]){
						aftertemp.c.integer = i;
						aftertemp.c.point5 = 1;
						break;
					}
				}
//				para_rcp->f = (aftertemp.f - nowtemp.f);						//算出箇所を移動 <20120924_watanabe_冷.79_華氏時の連動結果異常>
			}
			para_rcp->f = (aftertemp.f - nowtemp.f);							//<20120924_watanabe_冷.79_華氏時の連動結果異常>
		}
	}
	/*--*/
}

/**
 * @ingroup		application
 * @brief		自動二値有効/無効判定
 * @param[in]	ic_func_limit_rx - 最新値
 * @note		自動二値機能が有効か判定する
 * @warning		
 * @date		2015.12.21
 * @author		MSW S.Sugimoto
 */
UB apl_auto2_enable_jdg_fc(struct ic_func_limit ic_func_limit_rx)
{
	/*++自動二値の有効／無効を判定する*/
	UB ret_jdg_ac;		/* 判定結果(戻り値) */
	
	/*++自動二値が有効かチェックする*/
	if((DB_ENABLE == ic_func_limit_rx.automode)			/* 自動モード有？ */
	&& (DB_ENABLE == ic_func_limit_rx.auto2_mc)			/* かつ 自動二値有？ */
	&& (COOLER != ic_func_limit_rx.coolhpburn)			/* かつ 冷専機種でない？ */
	&& (HEATER != ic_func_limit_rx.coolhpburn)){		/* かつ 暖専機種でない？ */
		/* 自動二値有効 */
		ret_jdg_ac = DB_ENABLE;
	}else{
		/* 自動二値無効 */
		ret_jdg_ac = DB_DISABLE;
	}
	/*--*/
	/*--*/
	/*++判定結果を返却する*/
	return ret_jdg_ac;
	/*--*/
}

/**
 * @ingroup		application
 * @brief		華氏温度から摂氏温度を取得
 * @param[in]	temp_f - 最新値
 * @param[in]	*p_temp_c - 最新値
 * @note		
 * @warning		
 * @date		2015.12.21
 * @author		MSW S.Sugimoto
 */
UB apl_change_temp_f_from_c(UB temp_f, union temp* p_temp_c)
{
	/*++華氏を摂氏に変換する*/
	UB result = APL_TRUE;
	union temp convert_temp;
	
	/*++華氏→摂氏変換*/
	switch(temp_f)
	{
	case 37:
		convert_temp.c.integer = 3;
		convert_temp.c.point5 = 0;
		break;
	case 46:
		convert_temp.c.integer = 8;
		convert_temp.c.point5 = 0;
		break;
	case 53:
		convert_temp.c.integer = 12;
		convert_temp.c.point5 = 0;
		break;
	case 67:
		convert_temp.c.integer = 19;
		convert_temp.c.point5 = 0;
		break;
	case 68:
		convert_temp.c.integer = 20;
		convert_temp.c.point5 = 0;
		break;
	case 88:
		convert_temp.c.integer = 31;
		convert_temp.c.point5 = 0;
		break;
	case 89:
		convert_temp.c.integer = 32;
		convert_temp.c.point5 = 0;
		break;
	default:
		/*範囲外：温度未設定*/
		convert_temp.c.integer = TEMP_NONE;
		convert_temp.c.point5 = DB_DISABLE;
		result = APL_FALSE;
		break;
	}
	/* 計算結果を格納する */
	*p_temp_c = convert_temp;
	/*--*/
	/*--*/
	/*++処理結果を返却する*/
	return result;
	/*--*/
}

/**
 * @ingroup		application
 * @brief		温度範囲を0.5単位から1単位へ補正する
 * @param[in]	*temp_min - 範囲最小
 * @param[in]	 *temp_max - 範囲最大
 * @param[in]	 *temp05 - 0.5有効無効
 * @note		最小は切り上げ、最大は切り捨てする
 * @warning		
 * @date		2015.12.21
 * @author		MSW S.Sugimoto
 */
void apl_get_correct_temprange(union temp *temp_min, union temp *temp_max, UB temp05)
{
	/*++温度範囲を0.5単位から1単位へ補正する*/
	/*++0.5単位が無効かチェックする*/
	if(DB_DISABLE == temp05)
	{
		/*++最小値に0.5単位があるかチェックする*/
		if(0 != temp_min->c.point5)
		{
			/* 最小値に0.5単位があるとき、切り上げる */
			temp_min->c.integer++;
			temp_min->c.point5 = 0;
		}
		/*--*/
		/*++最大値に0.5単位があるかチェックする*/
		if(0 != temp_max->c.point5)
		{
			/* 最大値に0.5単位があるとき、切り捨てる */
			temp_max->c.point5 = 0;
		}
		/*--*/
	}
	/*--*/
	/*--*/
}

/**
 * @ingroup		application
 * @brief		最小温度差取得
 * @param[in]	fugou - 最新値
 * @param[in]	nowtemp - 最新値
 * @param[in]	*para_rcp - 最新値
 * @param[in]	temp_cf - 最新値
 * @param[in]	temp05_rc - 最新値
 * @note		最小は切り上げ、最大は切り捨てする
 * @warning		
 * @date		2015.12.21
 * @author		MSW S.Sugimoto
 */
void apl_settemp_diff_min_fv(UB fugou, union temp nowtemp, union temp *para_rcp,UB temp_cf, UB temp05_rc)
{
	/*++最小温度差を取得する*/
	/*++最新値の温度表示をチェックする*/
	if(TEMP_C == temp_cf ){
		/*++最新値に0.5単位があるかチェックする*/
		if(1 == para_rcp->c.point5)
		{
			/* 小数部を切り上げる */
			para_rcp->c.integer++;
			para_rcp->c.point5 = 0;
		}
		/*--*/
	}
	/*--*/
	
	/*++最新値の温度表示をチェックする*/
	else if(TEMP_F == temp_cf){
		/*++華氏時温度差を取得する*/
		apl_get_temp_diff_f_fv(fugou, nowtemp, para_rcp, temp05_rc);
		/*--*/
		/*++温度表示単位をチェックする*/
		if((DB_DISABLE == temp05_rc) && (1 == para_rcp->c.point5)) {
			/* 小数部を切り上げる */
			para_rcp->c.integer++;
			para_rcp->c.point5 = 0;
		}
		/*--*/
	}
	/*--*/
	
	/*++最新値の温度表示設定をチェックする*/
	else if(TEMP_05C == temp_cf){
		/*++温度表示単位をチェックする*/
		if((DB_DISABLE == temp05_rc) && (1 == para_rcp->c.point5)){
			/* 小数部を切り上げる */
			para_rcp->c.integer++;
			para_rcp->c.point5 = 0;
		}
		/*--*/
	} else {
		/* 処理なし */
	}
	/*--*/
	/*--*/
}

/**
 * @ingroup		application
 * @brief		操作設定B0/設定通報B0(マルチ)(M-NETアドレス設定用)取得
 * @param[in]	mnet_addr - アドレス設定(M-NETアドレス設定用(1～127))値
 * @param[in]	*p - M-NETアドレス
 * @return		戻り値 - 処理結果
 * @note		36MAから流用
 * @warning		なし
 * @date		2016.2.26
 * @author		SEC S.Sakamoto
 */
UB GET_MULTI_FUNCTION_ENABLE(UB mnet_addr,UW *p)
{
	/*++(マルチ/M-NETアドレス用)操作設定B0/設定通報B0受信値取得処理を行う*/
	UB rtn_ac = FALSE;
	UB work_index = 0;
	UW addr_mask_al = 0;
	UB tmp;
	
	/*++引数のmnet_addrの設定情報を取得する*/
	work_index = mnet_addr / 32;
	tmp = mnet_addr % 32;
	addr_mask_al = (UW)(0x00000001UL << tmp);
	if( ( p[work_index] & addr_mask_al ) != 0 ) {
		rtn_ac = TRUE;
	} else {
		rtn_ac = FALSE;
	}
	/*--*/
	/*++実行結果を返却する*/
	return rtn_ac;
	/*--*/
	/*--*/
}

/**
 * @ingroup		application
 * @brief		操作設定B0/設定通報B0(マルチ)(M-NETアドレス設定用)操作
 * @param[in]	mnet_addr - アドレス設定(M-NETアドレス設定用(1～127))値
 * @param[out]	*p - M-NETアドレス
 * @param[in]	enable - デモ機能有無
 * @return		戻り値 - 処理結果
 * @note		36MAから流用
 * @warning		なし
 * @date		2016.2.26
 * @author		SEC S.Sakamoto
 */
void SET_MULTI_FUNCTION_BIT_SET(UB mnet_addr,UW *p,UB enable)
{
	/*++(マルチ/M-NETアドレス用)操作設定B0/設定通報B0受信値設定処理を行う*/
	UB work_index = 0;
	
	/*++引数のmnet_addrの設定情報をenableに更新する*/
	work_index = mnet_addr / 32;
	
	if( DB_DISABLE == enable ){
		( p[work_index] ) &= ~((UW)( 0x00000001UL << ((UW)mnet_addr % 32) ));
	}else{
		( p[work_index] ) |= (UW)( 0x00000001UL << ((UW)mnet_addr % 32) );
	}
	/*--*/
	/*--*/
}

/**
 * @ingroup		application
 * @brief		操作設定B0/設定通報B0(マルチ)(M-NETアドレス設定用)操作
 * @param[in]	mnet_addr - アドレス設定(M-NETアドレス設定用(1～127))値
 * @param[out]	*p - M-NETアドレス
 * @param[in]	enable - デモ機能有無
 * @return		戻り値 - 処理結果
 * @note		36MAから流用
 * @warning		なし
 * @date		2016.2.26
 * @author		SEC S.Sakamoto
 */
void SET_MULTI_FUNCTION_ENABLE(UB mnet_addr,UW *p,UB enable)
{
	/*++(マルチ/M-NETアドレス用)操作設定B0/設定通報B0受信値設定処理を行う*/
	UB work_index = 0;
	UW tmp = (UW)enable;
	
	/*++引数のmnet_addrの設定情報を更新する*/
	work_index = mnet_addr / 32;
	( p[work_index] ) |= (UW)( tmp << ((UW)mnet_addr % 32) );
	/*--*/
	/*--*/
}

// ↓国内版44MA_GL17対応_MSW inoue_20211125
/**
 * @ingroup		application
 * @brief		操作設定B0/設定通報B0操作
 * @param[in]	addr_rc - アドレス設定値
 * @param[in]	受信値
 * @param[out]	*data_rlp - 格納領域
 * @return		戻り値 - 処理結果
 * @note		4bit分のデータをUW型の配列へ格納する
 * @warning		なし
 * @date		2021.11.25
 * @author		MSW inoue
 */
void SET_FUNCTION_BIT4_SET(UB addr_rc, UB param_rc, UW *data_rlp)
{
	/*++操作設定B0/設定通報B0受信値設定処理を行う*/
	UB data_idx_ac = 0;
	UB shft_cnt_ac = 0;
	
	/*++引数のmnet_addrの設定情報を更新する*/
	// 1byteあたリ2件保存 × 4byte分 = 8
	data_idx_ac = addr_rc / 8;	// データを保存する位置
	shft_cnt_ac = addr_rc % 8;	// 4bit分の左シフトを行う回数
	data_rlp[data_idx_ac] |= ((UW)param_rc << (4 * shft_cnt_ac));
	/*--*/
	/*--*/
}
// ↑国内版44MA_GL17対応_MSW inoue_20211125

/**
 * @ingroup		application
 * @brief		接続台数(マルチ)取得
 * @param[in]	*p - 有効アドレス
 * @return		戻り値 - 0～127:接続台数
 * @note		33MAAから流用
 * @warning		なし
 * @date		2017.6.29
 * @author		MSW K.Kawamura
 */
UB GET_MULTI_CONNECT_NUMBER(UW *p)
{
	UB loop;
	UB num_ac = 0;
	UB work_ac = 0;
	UW addr_mask_al = 0;
	UB tmp_ac;

	/*++(マルチ/M-NETアドレス用)接続台数取得処理を行う*/
	for(loop = MNET_ADDR_MIN ; loop <= MNET_ADDR_MAX ; loop++){
		/* 全アドレス分ループ */
		work_ac = loop / 32;
		tmp_ac = loop % 32;
		addr_mask_al = (UW)(0x00000001UL << tmp_ac);
		if((p[work_ac] & addr_mask_al) != 0){
			/* カウントアップ */
			num_ac++;
		}
	}
	/*--*/
	return num_ac;
}

/**
 * @ingroup		application
 * @brief		運転モード取得
 * param[in]	unitmode - 運転モード
 * @return		mode_type - 運転モード（冷房／暖房／自動）
 * @note		36MAから流用<br>
 				UMODE_XXXXからMODE_TYPE_XXXXを取得する
 * @warning		なし
 * @date		2016.2.26
 * @author		SEC S.Sakamoto
 */
UB apl_get_mode_type(UB unitmode)
{
	/*++UMODE_XXXXからMODE_TYPE_XXXXを取得する*/
	UB mode_type = 0;
	/*++現在の運転モードを判定する*/
	switch( unitmode ) 
	{
	case UMODE_COOL:
	case UMODE_DRY:
		/*++運転モードが冷房かドライの場合に運転モードタイプを設定する*/
		/*運転モードタイプを冷房にする*/
		mode_type = MODE_TYPE_COOL;
		break;
		/*--*/
	case UMODE_HEAT:
	case UMODE_BURNHEAT:
		/*++運転モードが暖房か燃焼暖房の場合に運転モードタイプを設定する*/
		/*運転モードタイプを暖房にする*/
		mode_type = MODE_TYPE_HEAT;
		break;
		/*--*/
	case UMODE_AUTOCOOL:
	case UMODE_AUTOHEAT:
	case UMODE_AUTO:
		/*++運転モードが自動冷房か自動暖房か自動の場合に運転モードタイプを設定する*/
		/*運転モードタイプを自動にする*/
		mode_type = MODE_TYPE_AUTO;
		break;
		/*--*/
	default:
		/*++運転モードが上記以外の場合に運転モードタイプを設定する*/
		/*運転モードタイプを冷房にする*/
		mode_type = MODE_TYPE_COOL;
		break;
		/*--*/
	}
	/*--*/
	/*--*/
	/*++運転モードタイプ設定値を返却する*/	
	return mode_type;
	/*--*/
}

/**
 * @ingroup		application
 * @brief		BLE機能有無取得
 * param[out]	*blefunc_rcp - 最新値
 * @return		戻り値 - APL_TRUE固定
 * @note		
 * @warning		
 * @date		2016.12.12
 * @author		MSW K.Kawamura
 */
UB aplutl_get_blefunc_fc(UB *blefunc_rcp)
{
#ifdef MODEL_MA
	UB func_ac;
#else	/* MODEL_ME */
#endif  /* MODEL_MA */

	/*++BLE機能有無を判定する*/
#ifdef MODEL_MA
	/*BSPからBLE機能有無を読み込む*/
	func_ac = ble_enable_get_fc();
	if(FALSE != func_ac){
		*blefunc_rcp = DB_ENABLE;
	}else{
		*blefunc_rcp = DB_DISABLE;
	}
#else	/* MODEL_ME */
	/*MEはBSP未実装のため無効固定*/
	*blefunc_rcp = DB_DISABLE;
#endif  /* MODEL_MA */
	/*--*/
	/*++取得結果を返却する(TRUE)*/
	return APL_TRUE;
	/*--*/
}

/**
 * @ingroup		application
 * @brief		Bluetooth接続情報取得
 * param[out]	*bleinfo_rxp - 最新値
 * @return		戻り値 - APL_TRUE固定
 * @note		
 * @warning		
 * @date		2016.12.12
 * @author		MSW K.Kawamura
 */
UB aplutl_get_bleinfo_fc(struct ble_info *bleinfo_rxp)
{
#ifdef USE_BLE
	struct ble_set bleset_ax;
	struct dsttype dsttype_ax;
	struct bdaddress bdadr_ax;

	/*++Bluetooth接続情報を取得する*/
	/*PINコードを設定*/
	read_db_bleset_fc(&bleset_ax);
	bleinfo_rxp->pincode_mu = bleset_ax.pincode_mu;
	/*リモコン形名を設定*/
	read_db_dsttype_fc(&dsttype_ax);
	memcpy((void *)bleinfo_rxp->id_mch, (const void *)dsttype_ax.data_mch, DSTTYPE_SIZE);
	/*BDアドレスを設定*/
	read_db_bdaddress_fc(&bdadr_ax);
//	memcpy((void *)bleinfo_rxp->bdadr_mch, (const void *)bdadr_ax.data_mch, BDADR_SIZE);
	hex2bytes_fc( bdadr_ax.data_mch, BLE_BD_ADDR_STR_LEN, bleinfo_rxp->bdadr_mch, 1 );
	/*--*/
#endif
	/*++取得結果を返却する(TRUE)*/
	return APL_TRUE;
	/*--*/
}

#ifdef COMPACT_MA
/**
 * @ingroup		application
 * @brief		タッチパネル補正情報アプリ変換
 * @param[in]	bsp_rx - BSP補正データ
 * @param[in]	apl_rxp - アプリ補正データ
 * @return		戻り値 - APL_TRUE固定
 * @note		
 * @warning		
 * @date		2016.7.24
 * @author		MSW K.Kawamura
 */
static UB aplutl_chg_bsp2apl_calibdata_fc(T_CALIB_DATA bsp_rx, struct calibration_set *apl_rxp)
{
	/*++タッチパネル補正情報をBSP用構造体からアプリ用構造体に設定する*/
	apl_rxp->data_muh[0][0] = bsp_rx.xi1;	// 左上のX座標
	apl_rxp->data_muh[0][1] = bsp_rx.yi1;	// 左上のY座標
	apl_rxp->data_muh[1][0] = bsp_rx.xi2;	// 中上のX座標
	apl_rxp->data_muh[1][1] = bsp_rx.yi2;	// 中上のY座標
	apl_rxp->data_muh[2][0] = bsp_rx.xi3;	// 右上のX座標
	apl_rxp->data_muh[2][1] = bsp_rx.yi3;	// 右上のY座標
	apl_rxp->data_muh[3][0] = bsp_rx.xi4;	// 左中のX座標
	apl_rxp->data_muh[3][1] = bsp_rx.yi4;	// 左中のY座標
	apl_rxp->data_muh[4][0] = bsp_rx.xi5;	// 中中のX座標
	apl_rxp->data_muh[4][1] = bsp_rx.yi5;	// 中中のY座標
	apl_rxp->data_muh[5][0] = bsp_rx.xi6;	// 右中のX座標
	apl_rxp->data_muh[5][1] = bsp_rx.yi6;	// 右中のY座標
	apl_rxp->data_muh[6][0] = bsp_rx.xi7;	// 左下のX座標
	apl_rxp->data_muh[6][1] = bsp_rx.yi7;	// 左下のY座標
	apl_rxp->data_muh[7][0] = bsp_rx.xi8;	// 中下のX座標
	apl_rxp->data_muh[7][1] = bsp_rx.yi8;	// 中下のY座標
	apl_rxp->data_muh[8][0] = bsp_rx.xi9;	// 右下のX座標
	apl_rxp->data_muh[8][1] = bsp_rx.yi9;	// 右下のY座標
	/*--*/

	/*++変換結果を返却する(TRUE)*/
	return APL_TRUE;
	/*--*/
}

/**
 * @ingroup		application
 * @brief		タッチパネル補正情報BSP変換
 * @param[in]	apl_rx - アプリ補正データ
 * @param[in]	bsp_rxp - BSP補正データ
 * @return		戻り値 - APL_TRUE固定
 * @note		
 * @warning		
 * @date		2016.7.24
 * @author		MSW K.Kawamura
 */
static UB aplutl_chg_apl2bsp_calibdata_fc(struct calibration_set apl_rx, T_CALIB_DATA *bsp_rxp)
{
	/*++タッチパネル補正情報をアプリ用構造体からBSP用構造体に設定する*/
	bsp_rxp->xi1 = apl_rx.data_muh[0][0];	// 左上のX座標
	bsp_rxp->yi1 = apl_rx.data_muh[0][1];	// 左上のY座標
	bsp_rxp->xi2 = apl_rx.data_muh[1][0];	// 中上のX座標
	bsp_rxp->yi2 = apl_rx.data_muh[1][1];	// 中上のY座標
	bsp_rxp->xi3 = apl_rx.data_muh[2][0];	// 右上のX座標
	bsp_rxp->yi3 = apl_rx.data_muh[2][1];	// 右上のY座標
	bsp_rxp->xi4 = apl_rx.data_muh[3][0];	// 左中のX座標
	bsp_rxp->yi4 = apl_rx.data_muh[3][1];	// 左中のY座標
	bsp_rxp->xi5 = apl_rx.data_muh[4][0];	// 中中のX座標
	bsp_rxp->yi5 = apl_rx.data_muh[4][1];	// 中中のY座標
	bsp_rxp->xi6 = apl_rx.data_muh[5][0];	// 右中のX座標
	bsp_rxp->yi6 = apl_rx.data_muh[5][1];	// 右中のY座標
	bsp_rxp->xi7 = apl_rx.data_muh[6][0];	// 左下のX座標
	bsp_rxp->yi7 = apl_rx.data_muh[6][1];	// 左下のY座標
	bsp_rxp->xi8 = apl_rx.data_muh[7][0];	// 中下のX座標
	bsp_rxp->yi8 = apl_rx.data_muh[7][1];	// 中下のY座標
	bsp_rxp->xi9 = apl_rx.data_muh[8][0];	// 右下のX座標
	bsp_rxp->yi9 = apl_rx.data_muh[8][1];	// 右下のY座標
	/*--*/

	/*++変換結果を返却する(TRUE)*/
	return APL_TRUE;
	/*--*/
}

/**
 * @ingroup		application
 * @brief		タッチパネル出荷検査補正情報取得
 * @param[in]	type_rc - 補正種別
 * @param[in]	calibdata_rx - タッチパネル補正データ
 * @return		戻り値 - 処理結果
 * @note		
 * @warning		
 * @date		2016.7.24
 * @author		MSW K.Kawamura
 */
UB aplutl_get_calibdata_shipment_fc(struct calibration_set *calibdata_rxp)
{
	UB rtn_ac = APL_TRUE;
	H bsprtn_aw = BSP_NG;
	T_CALIB_DATA bsp_ax;

	/*++BSPからタッチパネル出荷補正情報を読み出す*/
	bsprtn_aw = eeprom_read_fu(EEPROM_ADDR_SHIPMENT_CALIBRATION, (UB *)&bsp_ax, sizeof(bsp_ax));
	/*--*/
	if(BSP_OK ==  bsprtn_aw){
		/*++BSPデータからアプリデータに変換する*/
		rtn_ac = aplutl_chg_bsp2apl_calibdata_fc(bsp_ax, calibdata_rxp);
		/*--*/
	}else{
		rtn_ac = APL_FALSE;
	}
	/*++取得結果を返却する*/
	return rtn_ac;
	/*--*/
}

/**
 * @ingroup		application
 * @brief		タッチパネル補正情報設定
 * @param[in]	type_rc - 補正種別
 * @param[in]	calibdata_rx - タッチパネル補正データ
 * @return		戻り値 - 処理結果
 * @note		
 * @warning		
 * @date		2016.12.12
 * @author		MSW K.Kawamura
 */
UB aplutl_set_calibdata_fc(UB type_rc, struct calibration_set calibdata_rx)
{
	UB rtn_ac = APL_TRUE;
	UB bsprtn_ac;
	T_CALIB_DATA bsp_ax;

	/*++アプリデータからBSPデータに変換する*/
	aplutl_chg_apl2bsp_calibdata_fc(calibdata_rx, &bsp_ax);
	/*--*/
	/*++BSPにタッチパネル補正情報を書き込む*/
	if(CALIBTYPE_SHIPMENT == type_rc){
		/*出荷検査*/
		bsprtn_ac = shipment_calibrationdata_set_fc((const T_CALIB_DATA *)&bsp_ax);
	}else{
		/*通常*/
		bsprtn_ac = calibrationdata_set_fc((const T_CALIB_DATA *)&bsp_ax);
	}
	if(BSP_NG == bsprtn_ac){
		rtn_ac = APL_FALSE;
	}
	/*--*/
	/*++実行結果を返却する*/
	return rtn_ac;
	/*--*/
}

/**
 * @ingroup		application
 * @brief		座標の有効範囲判定
 * @param[in]	adx_ru - X方向のA/D変換値
 * @param[in]	ady_ru - Y方向のA/D変換値
 * @param[in]	pos_rc - キャリブレーションデータの位置
 * @return		戻り値 - 判定結果
 * @note		
 * @warning		
 * @date		2016.12.12
 * @author		MSW K.Kawamura
 */
UB aplutl_chk_calibdata_fc(UH adx_ru, UH ady_ru, UB pos_rc)
{
	UB rtn_ac = APL_TRUE;
	UB bsprtn_ac;

	/*++BSPで座標の有効範囲を判定する*/
	bsprtn_ac = calibration_check_range_fc(adx_ru, ady_ru, pos_rc);
	if(BSP_NG == bsprtn_ac){
		rtn_ac = APL_FALSE;
	}
	/*--*/
	/*++判定結果を返却する*/
	return rtn_ac;
	/*--*/
}

/**
 * @ingroup		application
 * @brief		シリアルデータ消去
 * @param[in]	adr_rl - フラッシュアドレス
 * @param[in]	size_rl - データサイズ
 * @return		戻り値 - 処理結果
 * @note		
 * @warning		
 * @date		2016.12.12
 * @author		MSW K.Kawamura
 */
UB aplutl_clr_serial_data_fc(UW adr_rl, UW size_rl)
{
	UB rtn_ac = APL_TRUE;
	B bsprtn_as;
	UB readrtn_ac;

	/*++シリアルフラッシュを消去する*/
	bsprtn_as = serialflash_start_fc();
	if(BSP_OK == bsprtn_as){
		readrtn_ac = serialflash_erase_fc(adr_rl, size_rl);
		if(BSP_OK != readrtn_ac){
			rtn_ac = APL_FALSE;
		}
		bsprtn_as = serialflash_end_fc();
		if(BSP_OK != bsprtn_as){
			rtn_ac = APL_FALSE;
		}
	}else{
		rtn_ac = APL_FALSE;
	}
	/*--*/
	/*++処理結果を返却する*/
	return rtn_ac;
	/*--*/
}


/**
 * @ingroup		application
 * @brief		シリアルデータ設定
 * @param[in]	adr_rl - フラッシュアドレス
 * @param[in]	*data_rcp - データポインタ
 * @param[in]	size_ru - データサイズ
 * @return		戻り値 - 処理結果
 * @note		
 * @warning		
 * @date		2016.12.12
 * @author		MSW K.Kawamura
 */
UB aplutl_set_serial_data_fc(UW adr_rl, UB *data_rcp, UH size_ru)
{
	UB rtn_ac = APL_TRUE;
	B bsprtn_as;
	UB readrtn_ac;

	/*++シリアルフラッシュに書き込む*/
	bsprtn_as = serialflash_start_fc();
	if(BSP_OK == bsprtn_as){
		readrtn_ac = serialflash_write_fc(adr_rl, (const UB *)data_rcp, size_ru);
		if(BSP_OK != readrtn_ac){
			rtn_ac = APL_FALSE;
		}
	}else{
		rtn_ac = APL_FALSE;
	}
	
	/*--*/
	/*++処理結果を返却する*/
	return rtn_ac;
	/*--*/
}

/**
 * @ingroup		application
 * @brief		シリアルデータ設定完了
 * @return		戻り値 - 処理結果
 * @note		
 * @warning		
 * @date		2016.12.12
 * @author		MSW K.Kawamura
 */
UB aplutl_fin_serial_data_fc(void)
{
	UB rtn_ac = APL_TRUE;
	B bsprtn_as;

	/*++シリアルフラッシュ書き込みを完了*/
	bsprtn_as = serialflash_end_fc();
	if(BSP_OK != bsprtn_as){
		rtn_ac = APL_FALSE;
	}
	/* シリアルフラッシュ書き込み関連処理が継続必要ならば実施する */
	if(BLE_LOG_PROCESS_DOING == ble_logo_process_stat_ec){
		/* ロゴ画像処理中であれば、処理を続行する */
		aplutl_ble_logo_process_after_save_fc();
	}else{
		/* 何もしない */
	}
	
	/*--*/
	/*++処理結果を返却する*/
	return rtn_ac;
	/*--*/
}

/**
 * @ingroup		application
 * @brief		シリアルデータ取得
 * @param[in]	adr_rl - フラッシュアドレス
 * @param[out]	*data_rcp - データポインタ
 * @param[in]	size_ru - データサイズ
 * @return		戻り値 - 処理結果
 * @note		
 * @warning		
 * @date		2016.12.12
 * @author		MSW K.Kawamura
 */
UB aplutl_get_serial_data_fc(UW adr_rl, UB *data_rcp, UH size_ru)
{
	UB rtn_ac = APL_TRUE;
	B bsprtn_as;
	UB readrtn_ac;

	/*++シリアルフラッシュを読み込む*/
	bsprtn_as = serialflash_start_fc();
	if(BSP_OK == bsprtn_as){
		readrtn_ac = serialflash_read_fc(adr_rl, data_rcp, size_ru);
		if(BSP_OK != readrtn_ac){
			rtn_ac = APL_FALSE;
		}
		bsprtn_as = serialflash_end_fc();
		if(BSP_OK != bsprtn_as){
			rtn_ac = APL_FALSE;
		}
	}else{
		rtn_ac = APL_FALSE;
	}
	/*--*/
	/*++処理結果を返却する*/
	return rtn_ac;
	/*--*/
}

/**
 * @ingroup		application
 * @brief		ExRomIconデータ(シリアルフラッシュ)クリア
 * @param[in]	id_rc - アイコンID
 * @return		戻り値 - 処理結果
 * @note		
 * @warning		
 * @date		2017.3.8
 * @author		MSW Hata
 */
UB aplutl_clr_ex_romicon_data_fc(UB id_rc)
{
	UB rtn_ac = APL_TRUE;
	struct ex_romicon_list romicon_list_ax;

	/*++ExRomIconデータ領域(シリアルフラッシュ)をクリアする*/
	/*DBからExRomIconリストを読み込む*/
	read_db_ex_romicon_list_fc(id_rc, &romicon_list_ax);
	if(DB_DISABLE != romicon_list_ax.ena_flag_mc){
		/*ExRomIcon削除リストを設定する*/
		write_db_ex_delicon_list_fc(id_rc, &romicon_list_ax);
		/*ExRomIconリストをクリアする*/
		romicon_list_ax = db_nv_common_initvalue.romicon_mxh[id_rc];
		write_db_ex_romicon_list_fc(id_rc, &romicon_list_ax);
		/* 不揮発記憶要求あり */
		updateinfo.eewrreq = 1;
	}else{
		/*シリアルフラッシュをクリア*/
		aplutl_clr_serial_data_fc(romicon_list_ax.flash_addr_ml, (UW)romicon_list_ax.wide_mu * (UW)romicon_list_ax.high_mu * PALLET_COLOR_SIZE);
	}
	/*--*/
	/*++処理結果を返却する*/
	return rtn_ac;
	/*--*/
}

#define ROMICON_DATA_BTES_MAX	(ROMICON_WIDE_LOGO * ROMICON_HIGH_LOGO * PALLET_COLOR_SIZE)
/**
 * @ingroup		application
 * @brief		ExRomIconデータ(シリアルフラッシュ)設定
 * @param[in]	id_rc - アイコンID
 * @param[in]	*data_rcp - データポインタ
 * @param[in]	index_rl - 書き込み先INDEX
 * @param[in]	size_ru - データサイズ
 * @return		戻り値 - 処理結果
 * @note		
 * @warning		
 * @date		2017.3.8
 * @author		MSW Hata
 */
UB aplutl_set_ex_romicon_data_fc(UB id_rc, UB *data_rcp, UW index_rl, UH size_ru)
{
	UB rtn_ac = APL_TRUE;
	struct ex_romicon_list romicon_list_ax;
	
	/*++ PROTECT：これ以上書き込むと領域オーバーになる場合は何もせず終わる */
	if( ROMICON_DATA_BTES_MAX < (index_rl + (UW)size_ru) ){		// index_rlは既に書込み済みのバイト数に相当、要求された書込みバイト数を足して領域オーバーするか判定
		rtn_ac = APL_FALSE;
		return rtn_ac;
	}
	/*--*/
	/*++ExRomIconデータ領域(シリアルフラッシュ)に書き込む*/
	/*DBからExRomIconリストを読み込む*/
	read_db_ex_romicon_list_fc(id_rc, &romicon_list_ax);
	/*シリアルフラッシュに設定する*/
	rtn_ac = aplutl_set_serial_data_fc(romicon_list_ax.flash_addr_ml + index_rl, data_rcp, size_ru);
	/*--*/
	/*++処理結果を返却する*/
	return rtn_ac;
	/*--*/
}

/**
 * @ingroup		application
 * @brief		PackBits圧縮データ解凍
 * @param[in]	data_rcp - 解凍対象データ
 * @param[in]	datasize_rl - 解凍対象データサイズ
 * @param[out]	buff_rcp - 解凍後データの格納先バッファ
 * @param[in]	buffsize_rl - 解凍後データの格納先バッファサイズ
 * @param[out]	decsize_rlp - 解凍後データのサイズ
 * @return 処理をしたデータサイズ(データ不正の場合は0)
 */
UW aplutl_decompress_fl(UB *data_rcp, UW datasize_rl, UB *buff_rcp, UW buffsize_rl, UW *decsize_rlp)
{
	UB count_ac;
	UB loop_ac;
	UW ret_al;
	UW remdata_al = datasize_rl; /* データの残りサイズ */
	UW rembuff_al = buffsize_rl; /* バッファの残りサイズ */

	/*++PackBitsで圧縮データを解凍する*/
	if((NULL != data_rcp) && (NULL != buff_rcp) && (NULL != decsize_rlp)){
		/* データ有効 */
		while(0 < remdata_al){
			/* データがなくなるまでループ */
			count_ac = *data_rcp;
			data_rcp++;
			if(0x80 == (count_ac & 0x80)){
				/* 繰り返しの場合 */
				count_ac = (UB)(~count_ac & 0xFF) + 2;
				if(3 > remdata_al){
					/* データ不足 */
					break;
				}
				if(((UW)count_ac * 2) > rembuff_al){
					/* バッファ不足 */
					break;
				}
				remdata_al--;
				/* データ展開 */
				for(loop_ac = 0 ; count_ac > loop_ac ; loop_ac++){
					*buff_rcp = data_rcp[0];
					buff_rcp++;
					*buff_rcp = data_rcp[1];
					buff_rcp++;
				}
				data_rcp = data_rcp + 2;
				remdata_al = remdata_al - 2;
				rembuff_al = rembuff_al - ((UW)count_ac * 2);
			}else{
				/* 繰り返しではない場合 */
				count_ac++;
				if((((UW)count_ac * 2) + 1) > remdata_al){
					/* データ不足 */
					break;
				}
				if(((UW)count_ac * 2) > rembuff_al){
					/* バッファ不足 */
					break;
				}
				remdata_al--;
				/* データ展開 */
				for(loop_ac = 0 ; count_ac > loop_ac ; loop_ac++){
					*buff_rcp = *data_rcp;
					buff_rcp++;
					data_rcp++;
					*buff_rcp = *data_rcp;
					buff_rcp++;
					data_rcp++;
				}
				remdata_al = remdata_al - ((UW)count_ac * 2);
				rembuff_al = rembuff_al - ((UW)count_ac * 2);
			}
		}
		*decsize_rlp = buffsize_rl - rembuff_al;
		ret_al = datasize_rl - remdata_al;
	}else{
		/* データ無効 */
		ret_al = 0;
	}
	/*--*/
	/*++データサイズを返却する*/
	return ret_al;
	/*--*/
}

/**
 * @ingroup		application
 * @brief		Exパレットデータ(シリアルフラッシュ)クリア
 * @param[in]	id_rc - パレットID
 * @return		戻り値 - 処理結果
 * @note		
 * @warning		
 * @date		2017.3.9
 * @author		MSW Hata
 */
UB aplutl_clr_ex_pallet_data_fc(UB id_rc)
{
	UB rtn_ac = APL_TRUE;
	struct ex_pallet_list pallet_list_ax;

	/*++Exパレットデータ領域(シリアルフラッシュ)をクリアする*/
	/*DBからExパレットリストを読み込む*/
	read_db_ex_pallet_list_fc(id_rc, &pallet_list_ax);
	if(DB_DISABLE != pallet_list_ax.ena_flag_mc){
		/*Exパレット削除リストを設定する*/
		write_db_ex_delpallet_list_fc(id_rc, &pallet_list_ax);
		/*Exパレットリストをクリアする*/
		pallet_list_ax = db_nv_common_initvalue.pallet_mxh[id_rc];
		write_db_ex_pallet_list_fc(id_rc, &pallet_list_ax);
		/* 不揮発記憶要求あり */
		updateinfo.eewrreq = 1;
	}else{
		/*シリアルフラッシュをクリア*/
		aplutl_clr_serial_data_fc(pallet_list_ax.flash_addr_ml, PALLET_SIZE_8 * PALLET_COLOR_SIZE);
	}
	/*--*/
	/*++処理結果を返却する*/
	return rtn_ac;
	/*--*/
}

/**
 * @ingroup		application
 * @brief		Exパレットデータ(シリアルフラッシュ)設定
 * @param[in]	id_rc - パレットID
 * @param[in]	*data_rcp - データポインタ
 * @return		戻り値 - 処理結果
 * @note		
 * @warning		
 * @date		2017.3.8
 * @author		MSW Hata
 */
UB aplutl_set_ex_pallet_data_fc(UB id_rc, UB *data_rcp)
{
	UB rtn_ac = APL_TRUE;
	struct ex_pallet_list pallet_list_ax;

	/*++Exパレットデータ領域(シリアルフラッシュ)に書き込む*/
	/*DBからExパレットリストを読み込む*/
	read_db_ex_pallet_list_fc(id_rc, &pallet_list_ax);
	/*シリアルフラッシュに設定する*/
	rtn_ac = aplutl_set_serial_data_fc(pallet_list_ax.flash_addr_ml, data_rcp, PALLET_SIZE_8 * PALLET_COLOR_SIZE);
	/*--*/
	/*++DBにExパレットリストを書き込む*/
	pallet_list_ax.ena_flag_mc = DB_ENABLE;
	rtn_ac = write_db_ex_pallet_list_fc(id_rc, &pallet_list_ax);
	/*--*/
	/* 不揮発記憶要求あり */
	updateinfo.eewrreq = 1;
	/*--*/
	/*++処理結果を返却する*/
	return rtn_ac;
	/*--*/
}

/**
 * @ingroup		application
 * @brief		Exパレットデータ取得
 * @param[in]	id_rc - パレットID
 * @param[out]	*data_rcp - データポインタ
 * @return		戻り値 - 処理結果
 * @note		
 * @warning		
 * @date		2016.12.12
 * @author		MSW K.Kawamura
 */
UB aplutl_get_ex_pallet_data_fc(UB id_rc, UB *data_rcp)
{
	UB rtn_ac = APL_TRUE;
	struct ex_pallet_list pallet_list_ax;

	/*++Exパレットデータを取得する*/
	/*DBからExパレットリストを読み込む*/
	read_db_ex_pallet_list_fc(id_rc, &pallet_list_ax);
	/*シリアルフラッシュから取得する*/
	rtn_ac = aplutl_get_serial_data_fc(pallet_list_ax.flash_addr_ml, data_rcp, PALLET_SIZE_8 * PALLET_COLOR_SIZE);
	/*--*/
	/*++処理結果を返却する*/
	return rtn_ac;
	/*--*/
}
#else	// SMART_MA or SMART_ME
#endif	// COMPACT_MA


#ifdef USE_BLE
// [BLE] 20170327 Miyazaki
/**
 * @ingroup		application
 * @brief		(BLE)PINコード生成初期化
 * @note		初期化のみでPINコード生成は行わない
 * @warning		
 * @date		2017.3.29
 * @author		MSW Miyazaki
 */
/* PINコード生成初期化 */
/* 初期化のみでPINコード生成は行わない */
void	ble_create_pin_init_fv( void )
{

	/*++ PINコード生成初期化 */
	struct serialno		serialno_ax;
	unsigned long		serial_num_al;
	
	/*++ 製造番号取得 */
	read_db_serialno_fc( &serialno_ax );		/* TRUEしか返らないので、戻り値処理不要 */
	/*--*/
	/*++ 製造番号の連番部5桁を数値化 */
	serial_num_al = natol_fl( &serialno_ax.data_mch[ 8 ], 5 );
	/*--*/
	/*++ PINコード生成のための擬似乱数列の初期値を設定 */
	srand( serial_num_al );
	/*--*/
	/*--*/
}


/**
 * @ingroup		application
 * @brief		(BLE)PINコード生成
 * @note		呼び出す毎に変わる乱数と製造番号からPINコードを生成する
 *				アプリのスケジュール機能で設定された時刻に本関数呼び出しを行い、PINコードを更新する
 * @warning		
 * @date		2017.3.29
 * @author		MSW Miyazaki
 */
unsigned long	ble_create_pin_code_fl( void )
{
	/*++PINコード生成*/
	unsigned long		pin_code_al;
	struct serialno		serialno_ax;
	unsigned long		serial_num_al;
	unsigned long		rand_num_al;
	
	/*++初期化 */
	pin_code_al = 0;
	serial_num_al = 0;
	rand_num_al = 0;
	/*--*/
	

	/*++製造番号取得 */
	read_db_serialno_fc( &serialno_ax );		/* TRUEしか返らないので、戻り値処理不要 */
	/*--*/
	/*++ 製造番号の連番部下2桁を数値化 */
	serial_num_al = natol_fl( &serialno_ax.data_mch[ ( 8 + 3 ) ], 2 );
	/*--*/
	/*++ PINコード生成のための乱数生成 */
	rand_num_al = rand();
	/*--*/
	/*++ 製造番号の連番部下2桁と乱数下2桁で4桁のPINコードを作る */
	pin_code_al = ( serial_num_al * 100 ) + ( rand_num_al % 100 );
	/*--*/
	/*++ 特殊値(0000or9999)になってしまったら補正をする */
	if( 0 == pin_code_al ){
		/*++ 0000は0001に補正する */
		pin_code_al++;
		/*--*/
	} else if( 9999 == pin_code_al ){
		/*++ 9999は9998に補正する */
		pin_code_al--;
		/*--*/
	} else {
		/* 0000,9999以外は補正なし */
	}
	/*--*/
	/*++ BCD変換 */
	pin_code_al = (UW)ev_Bin4Bcd_fu( (UH)pin_code_al );
	/*--*/
	/*--*/
	/*++ PINコードを返す */
	return pin_code_al;
	/*--*/
}


/**
 * @ingroup		application
 * @brief		文字列数値変換
 * @note		文字表記の数値をunsigned long型数値に変換する(文字数指定)
 *				文字列でない(末尾NULLが無い)ものや、文字列の一部を変換することが出来るよう、文字数指定方式にする
 *				10桁を超える場合はunsigned long型でも入り切らないが、プロテクトは難しいので成り行き任せとし、保証しない
 * @warning		
 * @date		2017.3.29
 * @author		MSW Miyazaki
 */
static unsigned long	natol_fl( UB *chars_rcp, UB size_rc )
{
	/*++文字表記の数値をunsigned long型数値に変換する*/
	unsigned long	ret_al;		/* 集計値(戻り値) */
	int				count_ai;
	UB				*top_acp;
	UB				one_char_ac;
	
	/*++ 初期化 */
	ret_al = 0;
	top_acp = chars_rcp;
	/*--*/
	
	/*++ 指定されたサイズ分、上位桁から順に文字→数値の変換を行う */
	for( count_ai = 0; count_ai < (int)size_rc; count_ai++ ){
		/*++ 一文字取り出す */
		one_char_ac = *top_acp;
		/*--*/
		/*++ 文字判定 */
		if ( ( '0' <= one_char_ac ) && ( '9' >= one_char_ac ) ){
			/*++ '0'～'9'なら、これまでの集計値を桁上げ(10倍)して、今回桁を1の位の数値として加える */
			ret_al = ret_al * 10;
			ret_al = ret_al + (unsigned long)( one_char_ac - '0' );
			/*--*/
		} else {
			/*++ '0'～'9'以外があったら、NULL文字同等と見做してここで終了 */
			break;
			/*--*/
		}
		/*--*/
		/*++ 参照位置を進める */
		top_acp++;
		/*--*/
	}
	/*--*/
	/*--*/
	/* 集計値を返す */
	return ret_al;
	/*--*/
}
// [BLE] 20170327 Miyazaki


/**************************************************************************************/
/**************************************************************************************/
/********************									*******************************/
/********************	デバイス名称、BDアドレス関連	*******************************/
/********************									*******************************/
/**************************************************************************************/
/**************************************************************************************/

/**
 *	@brief		デバイス名称組立
 *  @note		任意設定デバイス名称とBDアドレスの不揮発記憶からデバイス名を組み立てて返す
 *	@param[in]	UB *device_name_rcp		デバイス名称格納位置
 *	@return		デバイス名称のステータス
 *	@warning
 *	@date		2017.04.27
 *	@author		MSW Miyazaki
 **/
UB	aplutl_make_device_name_fc( UB *device_name_rcp )
{
	UB	ret_ac;
	UB	chk_ac;
	UB	stat_ac;
	UB	dev_name_work_ach[ BLE_DEV_NAME_SIZE ];
	struct bdaddress 	bd_addr_str_ax;
	
	/*++初期化 */
	stat_ac = BLE_DEV_NAME_STAT_DEF;
	memset( device_name_rcp, '\0', BLE_DEV_NAME_SIZE );
	/*--*/
	
	/*++デバイス名称が不揮発記憶されているか？ */
	ret_ac = read_db_devicename_fc( dev_name_work_ach );
	chk_ac = aplutl_device_name_chk_fc( dev_name_work_ach );
	if( ( APL_TRUE == ret_ac ) && ( CHECK_OK == chk_ac ) ){
		/* デバイス名称不揮発記憶あり */
		/*++不揮発から読み出したデバイス名を使う */
		strcpy( (char *)device_name_rcp, (char *)dev_name_work_ach );
		stat_ac = BLE_DEV_NAME_STAT_SET;
		/*--*/
	} else {
		/* デバイス名称不揮発記憶なし */
		/*++BDアドレスが不揮発記憶されているか？ */
		ret_ac = read_db_bdaddress_fc( &bd_addr_str_ax );
		chk_ac = aplutl_bd_addr_str_chk_fc( bd_addr_str_ax.data_mch );
		if( ( APL_TRUE == ret_ac ) && ( CHECK_OK == chk_ac ) ){
			/* BDアドレス不揮発記憶あり */
			/*++型名(7文字)＋"_"＋BDアドレス(12文字)をデバイス名称とする */
			strcpy( (char *)device_name_rcp, BLE_DEV_NAME_HED_STR );
			strcat( (char *)device_name_rcp, (char *)(bd_addr_str_ax.data_mch) );
			stat_ac = BLE_DEV_NAME_STAT_INIT;
			/*--*/
		}else{
			/* BDアドレス不揮発記憶なし */
			/*++仮のデバイス名称にする */
			strcpy( (char *)device_name_rcp, BLE_DEV_NAME_TMP_STR );
			stat_ac = BLE_DEV_NAME_STAT_DEF;
			/*--*/
		}
		/*--*/
	}
	/*--*/
	/*++デバイス名称のステータスを返す */
	return stat_ac;
	/*--*/
}


/**
 *	@brief		デバイス名称判定
 *  @note		デバイス名称が正常か否か判定する
 *	@param[in]	UB *device_name_rcp		デバイス名称格納位置
 *	@return		OK/NG
 *	@warning
 *	@date		2017.04.27
 *	@author		MSW Miyazaki
 **/
UB	aplutl_device_name_chk_fc( UB *device_name_rcp )
{
	UB	ret_ac;
	UB	count_ac;
	UB	one_char_ac;
	UB	*work_ptr_acp;
	
	/*++初期化 */
	ret_ac = CHECK_OK;
	count_ac = 0;
	work_ptr_acp = device_name_rcp;
	/*--*/
	
	/* 以下を有効文字とする										 */
	/* NULL=0x00												 */
	/* 半角スペース=0x20										 */
	/* 記号(!,",#,$,%,&,',(,).*,+,カンマ,-,ドット,/)=0x21-0x2F	 */
	/* 数字(0-9)=0x30-0x39										 */
	/* 記号(:,;,<,=,>,?)=0x3A-0x3F								 */
	/* 大文字アルファベット(A-Z)=0x41-0x5A						 */
	/* 記号([,\,],^,_,`)0x5B-0x60								 */
	/* 小文字アルファベット(a-z)=0x61-0x7A						 */
	/* 記号({,|,},~)=0x7B-0x7E									 */
	/* ※記号(@)=0x40は有効としない								 */
	
	/*++使用できない文字コードが含まれていないか判定する */
	while( BLE_DEV_NAME_SIZE > count_ac ){
		/*++1文字取り出す */
		one_char_ac = *work_ptr_acp;
		/*--*/
		/*++単文字判定 */
		if( '\0' == one_char_ac ){
			/*++null文字を見つけた場合 */
			if( 0 == count_ac ){
				/*++最初からnull文字(＝空文字列)は設定なしの意味でNGとする */
				ret_ac = CHECK_NG;
				/*--*/
			} else {
				/*++空文字列ではないときは正常 */
				ret_ac = CHECK_OK;
				/*--*/
			}
			/*--*/
			/*++ここで終わり */
			break;
			/*--*/
		} else if( ( '\x20' <= one_char_ac ) && ( '\x7E' >= one_char_ac ) && ( '\x40' != one_char_ac ) ){
			/*++正常文字範囲なら次の文字をチェック */
			count_ac++;
			work_ptr_acp++;
			/*--*/
		} else {
			/*++不正文字を見つけたらNGとしてここで終わり */
			ret_ac = CHECK_NG;
			break;
			/*--*/
		}
		/*--*/
	}
	/*--*/
	/*++末尾にNULLが無かった場合はNGに修正 */
	if( BLE_DEV_NAME_SIZE == count_ac ){
		ret_ac = CHECK_NG;
	}
	/*--*/
	
	/*++結果を返す */
	return ret_ac;
	/*--*/
}


/**
 *	@brief		BDアドレス文字列判定
 *  @note		BDアドレス文字列が正常か否か判定する
 *	@param[in]	UB *device_name_rcp		BDアドレス文字列格納位置
 *	@return		OK/NG
 *	@warning
 *	@date		2017.04.27
 *	@author		MSW Miyazaki
 **/
UB	aplutl_bd_addr_str_chk_fc( UB *bd_addr_str_rcp )
{
	UB	ret_ac;
	UB	count_ac;
	UB	one_char_ac;
	UB	*work_ptr_acp;
	
	/*++初期化 */
	ret_ac = CHECK_OK;
	count_ac = 0;
	work_ptr_acp = bd_addr_str_rcp;
	/*--*/
	
	/* 以下を有効文字とする										 */
	/* NULL=0x00(規定文字数後の場合のみ)						 */
	/* 数字(0-9)=0x30-0x39										 */
	/* 16進表記用アルファベット(A-F)=0x41-0x46					 */
	
	/*++使用できない文字コードが含まれていないか判定する */
	while( BLE_BD_ADDR_STR_SIZE > count_ac ){
		/*++1文字取り出す */
		one_char_ac = *work_ptr_acp;
		/*--*/
		/*++単文字判定 */
		if( '\0' == one_char_ac ){
			/*++null文字を見つけた場合 */
			if( (BLE_BD_ADDR_STR_SIZE - 1) > count_ac ){
				/*++長さゼロも含め、既定サイズに満たないときはNGとする */
				ret_ac = CHECK_NG;
				/*--*/
			} else {
				/*++既定サイズ末尾がnull文字のときは正常 */
				ret_ac = CHECK_OK;
				/*--*/
			}
			/*--*/
			/*++ここで終わり */
			break;
			/*--*/
		} else if( ( '0' <= one_char_ac ) && ( '9' >= one_char_ac ) ){
			/*++正常文字範囲なら次の文字をチェック */
			count_ac++;
			work_ptr_acp++;
			/*--*/
		} else if( ( 'A' <= one_char_ac ) && ( 'F' >= one_char_ac ) ){
			/*++正常文字範囲なら次の文字をチェック */
			count_ac++;
			work_ptr_acp++;
			/*--*/
		} else {
			/*++不正文字を見つけたらNGとしてここで終わり */
			ret_ac = CHECK_NG;
			break;
			/*--*/
		}
	}
	/*--*/
	/*++末尾にNULLが無かった場合はNGに修正 */
	if( BLE_BD_ADDR_STR_SIZE == count_ac ){
		ret_ac = CHECK_NG;
	}
	/*--*/
	
	/*++結果を返す */
	return ret_ac;
	/*--*/
}

// ↓国内版45MA_無線環境センサの検知データ送信_Masuda_20221215
/**
 *	@brief		BDアドレスが00-00-00-00-00-00かを判定
 *  @note		入力されたBDアドレスがすべて'0'(0x30)か判定する
 *	@param[in]	char *bd_addr_ascii_rsh		BDアドレス文字列格納位置。入力配列はASCII形式のデータ。
 *	@return		FALSE = 00-00-00-00-00-00		TRUE != 00-00-00-00-00-00
 *	@warning
 *	@date		2023.02.02
 *	@author		MESW Masuda
 **/
UB	aplutl_bd_addr_ascii_null_chk_fc( char *bd_addr_ascii_rsh )
{
	UB ret_ac=FALSE;
	UB char_cnt_ac;

	ret_ac = FALSE;																/* BD == 00-00-00-00-00-00 BDアドレス無効セット	*/
	for (char_cnt_ac = 0; BLE_BD_ADDR_STR_LEN > char_cnt_ac; char_cnt_ac++) {	// BDアドレス12桁分のみチェックする(末尾1文字分は判定しない)
		if ('0' != bd_addr_ascii_rsh[char_cnt_ac]) {
			ret_ac = TRUE;														/* BD != 00-00-00-00-00-00 BDアドレス有効セット	*/
			break;
		}
	}
	
	/*++結果を返す */
	return ret_ac;
	/*--*/
}
/**
 *	@brief		無線センサのBDアドレスがホワイトリストに登録されているか判定
 *  @note		入力されたBDアドレスがすべて'0'(0x30)か判定する
 *	@param[in]	char *bdaddr_rsh		BDアドレス文字列格納位置
 *	@return		DB_DISABLE=00-00-00-00-00-00	DB_ENABLE!=00-00-00-00-00-00
 *	@warning
 *	@date		2023.02.02
 *	@author		MESW Masuda
 **/
UB	aplutl_bd_wireless_sensor_registered_chk_fc( void )
{
	UB ret_ac=DB_DISABLE;
	UB sensor_registered_ac;
	UB wlst_bdaddress_ascii_ach[BLE_BD_ADDR_STR_LEN+1];	/* BDアドレス(ASCIIコード) */

	/*++DBからWhiteList登録BDアドレスを読み出し、DBのBLEセンサ登録情報 を取得する*/
	read_db_wlst_bdaddress_fc((struct bdaddress *)&wlst_bdaddress_ascii_ach);

	sensor_registered_ac = aplutl_bd_addr_ascii_null_chk_fc((char*)wlst_bdaddress_ascii_ach);
	if( TRUE == sensor_registered_ac ){
		ret_ac = DB_ENABLE;
	}
	/*--*/

	/*++結果を返す */
	return ret_ac;
	/*--*/
}
// ↑国内版45MA_無線環境センサの検知データ送信_Masuda_20221215

/**
 *	@brief		16進表記文字列変換
 *  @note		バイト数値配列を16進表記文字列に変換する
 *	@param[in]	UB *val_rcp		バイト数値配列先頭
 *				UB size_rc		数値バイト数
 *				UB *str_rcp		文字列出力先
 *				UB order_rc		順序(0:数値配列[0]を最上位とする/1:数値配列[0]を最下位とする)
 *	@return		変換後の文字数(正常終了なら数値バイト数×2)：末尾NULL含まない数
 *	@warning
 *	@date		2017.04.27
 *	@author		MSW Miyazaki
 **/
UB	bytes2hex_fc( UB *val_rcp, UB size_rc, UB *str_rcp, UB order_rc )
{
	UB			val_count_ac;
	UB			char_count_ac;
	UB			one_val_ac;
	UB			chg_work_ac;
	const UB	hex_char_tbl_ach[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	
	char_count_ac = 0;
	
	if( 0 == order_rc ){
		for( val_count_ac = 0; size_rc > val_count_ac; val_count_ac++ ){
			one_val_ac = val_rcp[ val_count_ac ];
			chg_work_ac = (one_val_ac >> 4) & 0x0f;
			str_rcp[ char_count_ac ] = hex_char_tbl_ach[ chg_work_ac ];
			char_count_ac++;
			chg_work_ac = one_val_ac & 0x0f;
			str_rcp[ char_count_ac ] = hex_char_tbl_ach[ chg_work_ac ];
			char_count_ac++;
		}
	} else {
		for( val_count_ac = size_rc; 0 < val_count_ac; val_count_ac-- ){
			one_val_ac = val_rcp[ ( val_count_ac - 1 ) ];
			chg_work_ac = (one_val_ac >> 4) & 0x0f;
			str_rcp[ char_count_ac ] = hex_char_tbl_ach[ chg_work_ac ];
			char_count_ac++;
			chg_work_ac = one_val_ac & 0x0f;
			str_rcp[ char_count_ac ] = hex_char_tbl_ach[ chg_work_ac ];
			char_count_ac++;
		}
	}
	str_rcp[ char_count_ac ] = '\0';
	
	return char_count_ac;
}


/**
 *	@brief		16進表記文字列変換
 *  @note		16進表記文字列をバイト数値配列に変換する
 *	@param[in]	UB *str_rcp		文字列格納位置
 *				UB len_rc		文字列長さ
 *				UB *str_rcp		バイト数値配列出力先
 *				UB order_rc		順序(0:数値配列[0]を最上位とする/1:数値配列[0]を最下位とする)
 *	@return		変換後のバイト数(正常終了なら文字列長さ/2)
 *	@warning
 *	@date		2017.04.27
 *	@author		MSW Miyazaki
 **/
UB	hex2bytes_fc( UB *str_rcp, UB len_rc, UB *val_rcp, UB order_rc )
{
	UB			val_count_ac;
	UB			char_count_ac;
	UB			one_char_ac;
	UB			chg_work_ac;
	UB			val_size_ac;
	
	char_count_ac = 0;
	val_size_ac = (len_rc / 2);
	if( 0 == order_rc ){
		val_count_ac = 0;
	} else {
		val_count_ac = val_size_ac - 1;
	}
	
	while( len_rc > char_count_ac ){
		one_char_ac = str_rcp[ char_count_ac ];
		if( '9' >= one_char_ac ){
			chg_work_ac = (UB)( one_char_ac - '0' ) << 4;
		} else {
			chg_work_ac = (UB)( one_char_ac - 'A' + 10 ) << 4;
		}
		char_count_ac++;
		one_char_ac = str_rcp[ char_count_ac ];
		if( '9' >= one_char_ac ){
			chg_work_ac = chg_work_ac + ( one_char_ac - '0' );
		} else {
			chg_work_ac = chg_work_ac + ( one_char_ac - 'A' + 10 );
		}
		char_count_ac++;
		val_rcp[ val_count_ac ] = chg_work_ac;
		if( 0 == order_rc ){
			val_count_ac++;
		} else {
			val_count_ac--;
		}
	}
	
	return val_size_ac;
}

/* ############################################################################################################### */
/* ############################################################################################################### */
/* ############################################################################################################### */
/* ############################################################################################################### */
#ifdef COMPACT_MA
void	aplutl_set_logo_pic_info_fv( UH size_ru, UB *data_rcp )
{
	aplutil_logo_pic_data_size_eu = size_ru;
	aplutil_logo_pic_data_ecp = data_rcp;
	aplutil_logo_pic_data_read_index_eu = 0;
}

void aplutl_logo_pic_set_complete_fv(void){
	/*++ExRomIcon画像設定終了を実行する*/
	struct ex_romicon_list romicon_list_ax;

	/*++DBからExRomIconリストを読み込む*/
	read_db_ex_romicon_list_fc(ROMICON_ID_LOGO, &romicon_list_ax);
	/*--*/
	/*++DBにExRomIconリストを書き込む*/
	romicon_list_ax.ena_flag_mc = DB_ENABLE;
	write_db_ex_romicon_list_fc(ROMICON_ID_LOGO, &romicon_list_ax);
	/*--*/
	/* 不揮発記憶要求あり */
	updateinfo.eewrreq = 1;
	/*--*/

}
	
	
void	aplutl_logo_pic_extract_and_save_fc( void )
{
	UB	data_count_info_ac;
	UB	decompable_ac;
	UB	ret_ac;
	UH	loop_bytes_au;
	UH	count_au;
	
	/* この処理に来た時点でFLASH書込み実行中にしておく */
	ble_logo_process_stat_ec = BLE_LOG_PROCESS_DOING;
	
	if(0 < aplutil_logo_pic_data_size_eu){
		/* 受信データがある場合：セッションDATAによる要求 */
		/* 受信データを読み切るまで繰り返し */
		while( aplutil_logo_pic_data_read_index_eu < aplutil_logo_pic_data_size_eu ){
				/* 切り出しサイズを決める */
				if(0 == ble_hotel_logo_pic_comp_unit_buf_ex.total_bytes_mu){
					/* 空 */
					/* 最初に解凍後データ数情報を切り出す */
					data_count_info_ac = aplutil_logo_pic_data_ecp[aplutil_logo_pic_data_read_index_eu];
					aplutil_logo_pic_data_read_index_eu++;
					ble_hotel_logo_pic_comp_unit_buf_ex.data_count_info_mc = data_count_info_ac;
					ble_hotel_logo_pic_comp_unit_buf_ex.total_bytes_mu = 1;
					if( 0 != ( data_count_info_ac & 0x80) ){
						/* 最上位bitがON(負値)のときは「連続数」 */
						/* 続く2byteが切り出しバイト数 */
						loop_bytes_au = PALLET_COLOR_SIZE;
					} else {
						/* 最上位bitがOFF(正値)のときは「非連続数」 */
						/* (この値+1)*2が切り出しバイト数：最大128*2=256 */
						loop_bytes_au = ((UH)data_count_info_ac + 1) * PALLET_COLOR_SIZE;
					}
				} else {
					/* 空ではない：前回受信の末尾が１圧縮単位に満たなかった場合 */
					/* 解凍後データ数情報は既に切り出されて格納されているはずなので、それから連続/非連続と残りの切り出し数を得る */
					data_count_info_ac = ble_hotel_logo_pic_comp_unit_buf_ex.data_count_info_mc;
					if( 0 != ( data_count_info_ac & 0x80) ){
						/* 最上位bitがON(負値)のときは「連続数」 */
						/* 既に格納されているデータバイト数がデータ数2byteに不足する分を切り出しバイト数とする：解凍後データ数情報分の1byteも減算 */
						loop_bytes_au = PALLET_COLOR_SIZE - (ble_hotel_logo_pic_comp_unit_buf_ex.total_bytes_mu - 1);
					} else {
						/* 最上位bitがOFF(正値)のときは「非連続数」 */
						/* 既に格納されているデータバイト数が(この値+1)*2に不足する分が切り出しバイト数：最大128*2=256：解凍後データ数情報分の1byteも減算 */
						loop_bytes_au = (((UH)data_count_info_ac + 1) * PALLET_COLOR_SIZE) - (ble_hotel_logo_pic_comp_unit_buf_ex.total_bytes_mu - 1);
					}
				}
				
				/* 切り出し */
				decompable_ac = TRUE;	// 解凍可能
				for( count_au = 0; count_au < loop_bytes_au; count_au++){
					if( aplutil_logo_pic_data_read_index_eu >= aplutil_logo_pic_data_size_eu ){
						/* 切り出し途中で受信データバッファの末尾に達した場合は、次受信を待たないと切り出し継続できない */
						/* 従って、今回途中まで切り出した分は解凍できない */
						decompable_ac = FALSE;	// 解凍不可
						break;
					} else {
						/* 1byte切り出して１圧縮単位バッファに入れる */
						ble_hotel_logo_pic_comp_unit_buf_ex.data_mch[ (ble_hotel_logo_pic_comp_unit_buf_ex.total_bytes_mu - 1) ] =  aplutil_logo_pic_data_ecp[ aplutil_logo_pic_data_read_index_eu ];
						ble_hotel_logo_pic_comp_unit_buf_ex.total_bytes_mu++;
					}
					aplutil_logo_pic_data_read_index_eu++;
				}
				
				/* 解凍 */
				if( TRUE == decompable_ac ){
					/* 解凍可能 */
					aplutl_logo_pic_decomp_one_unit_fv();
					/* １圧縮単位バッファをクリア */
					ble_hotel_logo_pic_comp_unit_buf_ex.total_bytes_mu = 0;
				} else {
					/* 解凍不可 */
					/* ここに来るのは受信データ末尾が途中で切れている場合 */
					/* １回の受信分の処理は完了しており、FLASH書込みは発生しないので、FLASH書込み完了にして終了 */
					ble_logo_process_stat_ec = BLE_LOG_PROCESS_FINISHED;
					return;
				}
				
				/* FLASH書込み */
				if( ble_hotel_logo_pic_decomp_buf_ex.bytes_mu >= BLE_HOTEL_LOGO_PIC_SAVE_UNIT_SIZE ){
					/* FLASH書込みサイズに達している */
					/* シリアルフラッシュ書き込み処理 */
					ret_ac = aplutl_set_ex_romicon_data_fc( ROMICON_ID_LOGO, ble_hotel_logo_pic_decomp_buf_ex.data_mch, ( ble_logo_save_count_el * BLE_HOTEL_LOGO_PIC_SAVE_UNIT_SIZE ), BLE_HOTEL_LOGO_PIC_SAVE_UNIT_SIZE );
					if( APL_TRUE == ret_ac ){
						/* FLASH書込み実行中にして終了 */
						ble_logo_process_stat_ec = BLE_LOG_PROCESS_DOING;
					} else {
						/* フラッシュ書き込みできない場合 */
						/* FLASH書込み完了にして終了 */
						ble_logo_process_stat_ec = BLE_LOG_PROCESS_FINISHED;
					}
					return;
				} else {
					/* FLASH書込みサイズに達していない */
					/* 次の１圧縮単位の切り出しを行う */
				}
		}
		
		/* 途中returnせずにここに至る場合は、以下のケース */
		/* (A)受信データ全て解凍したがFLASH書込みサイズに達しないためFLASH書込みせずループ終了 */
		/* (B)受信データ末尾で丁度FLASH書込みサイズに達し、コールバックから継続実行しようとした場合：ループ内を１回も実行せずここに来る */
		/* (D)何かの間違いでループ終了条件に至った場合：処理継続不能 */
		ble_logo_process_stat_ec = BLE_LOG_PROCESS_FINISHED;
		return;
	} else {
		/* 受信データがない場合：セッション終了による最終端数の書き込み要求 */
		/* 切り出しも解凍も行わず、解凍バッファに残っている分をFLASH書込みする */
		if( 0 < ble_hotel_logo_pic_decomp_buf_ex.bytes_mu ){
			/* シリアルフラッシュ書き込み処理 */
			ret_ac = aplutl_set_ex_romicon_data_fc( ROMICON_ID_LOGO, ble_hotel_logo_pic_decomp_buf_ex.data_mch, ( ble_logo_save_count_el * BLE_HOTEL_LOGO_PIC_SAVE_UNIT_SIZE ), ble_hotel_logo_pic_decomp_buf_ex.bytes_mu );
			if( APL_TRUE == ret_ac ){
				/* FLASH書込み実行中にして終了 */
				ble_logo_process_stat_ec = BLE_LOG_PROCESS_DOING;
			} else {
				/* フラッシュ書き込みできない場合 */
				/* FLASH書込み完了にして終了 */
				ble_logo_process_stat_ec = BLE_LOG_PROCESS_FINISHED;
			}
		} else {
			/* 進捗率100%にする */
			aplutl_set_logo_pic_process_progress_rate_fv( 100 );
			/* バッファ類の初期化をし、FLASH書込み完了にして終了 */
			aplutl_logo_pic_process_initialize_fv();
			/* ロゴ画像完了処理を実施する */
			aplutl_logo_pic_set_complete_fv();
			/* FLASH書込み完了にして終了 */
			ble_logo_process_stat_ec = BLE_LOG_PROCESS_FINISHED;
			/*調停コントローラを動かす	*/
			start_arbitration_controller();
		}
		return;
	}
}

void	aplutl_logo_pic_decomp_one_unit_fv( void )
{
	UB count_ac;
	UB loop_ac;
	UH index_au;
	
	/*++PackBitsで圧縮データを解凍する*/
	count_ac = ble_hotel_logo_pic_comp_unit_buf_ex.data_count_info_mc;
	if(0x80 == (count_ac & 0x80)){
		/* 繰り返しの場合 */
		count_ac = (UB)(~count_ac & 0xFF) + 2;		// -1～-128(0xFF～0x80)→2～129
		/* 保険：間違ってもバッファオーバーして書込みをしないようにする */
		if( ( BLE_HOTEL_LOGO_PIC_DECOMP_BUF_SIZE - ble_hotel_logo_pic_decomp_buf_ex.bytes_mu ) < ( count_ac * 2 ) ){
			/* 解凍バッファの残byte数より追加しようとするbyte数が多い場合は処理しない */
			return;
		}
		/* データ展開 */
		for(loop_ac = 0 ; count_ac > loop_ac ; loop_ac++){
			ble_hotel_logo_pic_decomp_buf_ex.data_mch[ble_hotel_logo_pic_decomp_buf_ex.bytes_mu] = ble_hotel_logo_pic_comp_unit_buf_ex.data_mch[0];
			ble_hotel_logo_pic_decomp_buf_ex.bytes_mu++;
			ble_hotel_logo_pic_decomp_buf_ex.data_mch[ble_hotel_logo_pic_decomp_buf_ex.bytes_mu] = ble_hotel_logo_pic_comp_unit_buf_ex.data_mch[1];
			ble_hotel_logo_pic_decomp_buf_ex.bytes_mu++;
		}
	}else{
		/* 繰り返しではない場合 */
		count_ac++;								// 0～127(0x00～0x7F)→1～128
		/* 保険：間違ってもバッファオーバーして書込みをしないようにする */
		if( ( BLE_HOTEL_LOGO_PIC_DECOMP_BUF_SIZE - ble_hotel_logo_pic_decomp_buf_ex.bytes_mu ) < ( count_ac * PALLET_COLOR_SIZE ) ){
			/* 解凍バッファの残byte数より追加しようとするbyte数が多い場合は処理しない */
			return;
		}
		/* データ展開 */
		index_au = 0;
		for(loop_ac = 0 ; count_ac > loop_ac ; loop_ac++){
			ble_hotel_logo_pic_decomp_buf_ex.data_mch[ble_hotel_logo_pic_decomp_buf_ex.bytes_mu] = ble_hotel_logo_pic_comp_unit_buf_ex.data_mch[index_au];
			ble_hotel_logo_pic_decomp_buf_ex.bytes_mu++;
			index_au++;
			ble_hotel_logo_pic_decomp_buf_ex.data_mch[ble_hotel_logo_pic_decomp_buf_ex.bytes_mu] = ble_hotel_logo_pic_comp_unit_buf_ex.data_mch[index_au];
			ble_hotel_logo_pic_decomp_buf_ex.bytes_mu++;
			index_au++;
		}
	}
	/*--*/
}

void	aplutl_logo_pic_process_initialize_fv( void )
{
	UB ble_logo_save_rate_ac = 0;
	
	/* ロゴ画像受信データ */
	aplutil_logo_pic_data_size_eu = 0;
	aplutil_logo_pic_data_read_index_eu = 0;
	
	/* １圧縮単位バッファ */
	ble_hotel_logo_pic_comp_unit_buf_ex.total_bytes_mu = 0;
	
	/* 解凍バッファ */
	ble_hotel_logo_pic_decomp_buf_ex.bytes_mu = 0;
	
	/* ロゴ画像書き込み回数 */
	ble_logo_save_count_el = 0;
	
	/* 進捗率 */
	write_db_bleupstatus_fc(&ble_logo_save_rate_ac);
}

void aplutl_ble_logo_process_after_save_fc(void)
{
	UB	new_rate_ac = 0;
	
	/* セッションDATAによる書込み継続動作からのコールバックか、セッション終了による末端端数分保存後のコールバックかを判定 */
	/* 解凍バッファの格納バイト数で判断 */
	if( ble_hotel_logo_pic_decomp_buf_ex.bytes_mu >= BLE_HOTEL_LOGO_PIC_SAVE_UNIT_SIZE ){
		/* 解凍バッファにFLASH書込みサイズ以上のデータがある場合は、セッションDATAによる書込み継続動作からのコールバック */
		/* 解凍バッファの後始末をする */
		UB	*src_acp;
		UH	move_size_au;
		
		/* 書込み数を1UP */
		ble_logo_save_count_el++;
		
		/* 進捗率更新 */
		new_rate_ac = aplutl_calc_logo_pic_process_progress_rate_fc( ble_logo_save_count_el );
		aplutl_set_logo_pic_process_progress_rate_fv( new_rate_ac );
		
		/* コピー元位置とコピーサイズを計算：コピー先はバッファ先頭なので計算不要 */
		src_acp = ble_hotel_logo_pic_decomp_buf_ex.data_mch + BLE_HOTEL_LOGO_PIC_SAVE_UNIT_SIZE;
		move_size_au = ble_hotel_logo_pic_decomp_buf_ex.bytes_mu - BLE_HOTEL_LOGO_PIC_SAVE_UNIT_SIZE;
		
		/* コピー領域が重なることは無いので、memcpyを使う */
		memcpy( ble_hotel_logo_pic_decomp_buf_ex.data_mch, src_acp, move_size_au );
		ble_hotel_logo_pic_decomp_buf_ex.bytes_mu = move_size_au;
		/* 引き続き、切り出し＆解凍＆FLASH保存を実施 */
		aplutl_logo_pic_extract_and_save_fc();
	} else {
		/* 解凍バッファにFLASH書込みサイズ未満のデータしかない場合は、セッション終了による末端端数分保存によるコールバック */
		/* 進捗率100%にする */
		aplutl_set_logo_pic_process_progress_rate_fv( 100 );
		
		/* バッファ類の初期化 */
		aplutl_logo_pic_process_initialize_fv();
		/* ロゴ画像完了処理を実施する */
		aplutl_logo_pic_set_complete_fv();
		/* FLASH書込み完了にして終了 */
		ble_logo_process_stat_ec = BLE_LOG_PROCESS_FINISHED;
		/*調停コントローラを動かす	*/
		start_arbitration_controller();
	}
}


#define BLE_HOTEL_LOGO_PIC_SAVE_UNIT_PIX	(BLE_HOTEL_LOGO_PIC_SAVE_UNIT_SIZE / PALLET_COLOR_SIZE )									/* FLASH書込単位pixel数(バイト数から換算) */
#define LOGO_PIC_SAVE_PROGRESS_10PER_PIX	6400																						/* 進捗率+10％に相当するPixel数 */
#define LOGO_PIC_SAVE_PROGRESS_10PER_COUNT	(LOGO_PIC_SAVE_PROGRESS_10PER_PIX / BLE_HOTEL_LOGO_PIC_SAVE_UNIT_PIX)						/* 進捗率+10％に相当するFLASH書き込み回数 */

UB	aplutl_calc_logo_pic_process_progress_rate_fc( UW count_rl )
{
	UB	rate_ac;			/* 進捗率(％)：戻り値 */
	
	/*++ 書込み中(消去終了時点で０回目)は、書き込み回数を進捗率に換算 */
	rate_ac = 10 * ((UB)(count_rl / LOGO_PIC_SAVE_PROGRESS_10PER_COUNT) + 1);
	/*--*/
	
	return rate_ac;
}
void	aplutl_set_logo_pic_process_progress_start_fv( void )
{
	/* ロゴ画像スタートをGUIへ通知 */
	
	/*++ 進捗は0% */
	UB rate = 0;
	/*--*/
	/*++ データ更新 */
	write_db_bleupstatus_fc(&rate);
	/*--*/
	/*++ GUIへ更新通知 */
	snd_gui_event_msg(IMM_EV_BLE_PICT_UPDATE_START);
	/*--*/
}

void	aplutl_set_logo_pic_process_progress_rate_fv( UB rate_rc )
{
	/*++ 進捗率変化あれば更新 */
	UB ble_logo_save_rate_ac;
	read_db_bleupstatus_fc(&ble_logo_save_rate_ac);
	if( ble_logo_save_rate_ac != rate_rc ){
		/*++ 画面更新通知か、画面終了通知かを判定 */
		if( 100 > rate_rc ){
			/*++ データ更新 */
			write_db_bleupstatus_fc(&rate_rc);
			/*--*/
			/*++ GUIへ更新通知（更新） */
			snd_gui_event_msg(IMM_EV_BLE_SCREEN_UPDATE);
			/*--*/
		} else {
			/*++ 100%になったら0%にクリア */
			rate_rc = 0;
			write_db_bleupstatus_fc(&rate_rc);
			/*--*/
			/*++ GUIへ終了通知 */
			snd_gui_event_msg(IMM_EV_BLE_PICT_UPDATE_END);
			/*--*/
		}
		/*--*/
	}
	/*--*/
}
#endif //COMPACT_MA


/**
 * @ingroup		application
 * @brief		
 * @param[in]	
 * @return		なし
 * @note		BLEMWからのBLE切断通知時に必要な処理を実施する
 * @warning		なし
 * @date		2017.07.04
 * @author		MSW Tomomatsu
 */
void apl_exec_ble_disconnect_fv(void)
{
	/* BLE切断をGUIへ通知する */
	snd_gui_event_msg(IMM_EV_BLE_DISCONNECT);
}

	/**
 * @ingroup		application
 * @brief		
 * @param[in]	
 * @return		なし
 * @note		BLE通信種別の通知
 * @warning		なし
 * @date		2017.07.07
 * @author		MSW Tomomatsu
 */
void apl_send_blesessiontype_phase_fv(UB blecomkind_rc)
{
	/*++ 変数定義 */
	UB gui_event_ac = IMM_EV_BLE_LOGIN_START;	 /* 初期値にセッションスタートを入れる */
	UB check_event_ac = APL_FALSE;				 /* 作業変数 */
	UB loop_ac = 0;								 /* 作業変数 */
	/*--*/
	
	/*++ BLE通信種別をGUIへのイベントへ変換する */
	for( loop_ac = 0 ; loop_ac < BLECOM_SESSSIONPHASETYPE_MAX; loop_ac++ ){
		if(blecomtype_cnv_guievent_tbl[loop_ac].blecomtype_mc == blecomkind_rc){
			/* BLE受信種別が一致 */
			gui_event_ac = blecomtype_cnv_guievent_tbl[loop_ac].guievent_mc;
			/* GUIイベントあり */
			check_event_ac = APL_TRUE;
			break;
		}
	}
	/*--*/
	
	/*++ 定義されたイベントであればBLE受信のイベントをGUIへ通知する */
	if(APL_TRUE == check_event_ac){
		/* GUIへイベントを通知する */
		snd_gui_event_msg(gui_event_ac);
	}else{
		/* 何もしない*/
	}
	/*--*/
}

#endif //USE_BLE


#ifdef MODEL_MA							/* 2017/6/27 MSW ogawa #6782 */
/* 異常コード変換テーブル */
struct err_cnv {
	UB			atr;					/* 異常機器属性					*/	//[A_033 NS_KOYAMA 080814]
	UH			codelong;				/* ４桁異常コード				*/	//[A_033 NS_KOYAMA 080814]
	UB			codeshort;				/* ２桁異常コード				*/	//[A_033 NS_KOYAMA 080814]
};
const struct err_cnv err_cnv_tbl[]  = {										//[A_033 NS_KOYAMA 080814]
	{ERRATR_IC,		0x6831,	(EC_Ex + EC_x4),},
	{ERRATR_IC,		0x6834,	(EC_Ex + EC_x4),},
	{ERRATR_IC,		0x6832,	(EC_Ex + EC_x5),},
	{ERRATR_IC,		0x6833,	(EC_Ex + EC_x5),},
	{ERRATR_IC,		0x6840,	(EC_Ex + EC_x6),},
	{ERRATR_IC,		0x6843,	(EC_Ex + EC_x6),},
	{ERRATR_IC,		0x6841,	(EC_Ex + EC_x7),},
	{ERRATR_IC,		0x6842,	(EC_Ex + EC_x7),},
	{ERRATR_OC,		0x6840,	(EC_Ex + EC_x8),},
	{ERRATR_OC,		0x6843,	(EC_Ex + EC_x8),},
	{ERRATR_OC,		0x6841,	(EC_Ex + EC_x9),},
	{ERRATR_OC,		0x6842,	(EC_Ex + EC_x9),},
	{ERRATR_OC,		0x6844,	(EC_Ex + EC_xA),},
	{ERRATR_OC,		0x6845,	(EC_Ex + EC_xB),},
	{ERRATR_OC,		0x6846,	(EC_Ex + EC_xC),},
	{ERRATR_OC,		0x0403,	(EC_Ex + EC_xD),},
	{ERRATR_OC,		0x1108,	(EC_Ux + EC_x2),},
	{ERRATR_OC,		0x1102,	(EC_Ux + EC_x2),},
	{ERRATR_IC,		0x1110,	(EC_Px + EC_x8),},
	{ERRATR_OC,		0x5104,	(EC_Ux + EC_x3),},
	{ERRATR_OC,		0x5105,	(EC_Ux + EC_x4),},
	{ERRATR_OC,		0x4101,	(EC_Ux + EC_x6),},
	{ERRATR_OC,		0x4117,	(EC_Ux + EC_xC),},
	{ERRATR_OC,		0x1504,	(EC_Ux + EC_xD),},
	{ERRATR_OC,		0x1302,	(EC_Ux + EC_xE),},
	{ERRATR_OC,		0x4100,	(EC_Ux + EC_xF),},
	{ERRATR_OC,		0x5300,	(EC_Ux + EC_xH),},
	{ERRATR_OC,		0x1300,	(EC_Ux + EC_xL),},
	{ERRATR_OC,		0x4103,	(EC_Fx + EC_x1),},
	{ERRATR_OC,		0x4102,	(EC_Fx + EC_x2),},
	{ERRATR_OC,		0x5202,	(EC_Fx + EC_x3),},
	{ERRATR_OC,		0x5201,	(EC_Fx + EC_x5),},
	{ERRATR_OC,		0x4118,	(EC_Fx + EC_x7),},
	{ERRATR_OC,		0x4115,	(EC_Fx + EC_x8),},
	{ERRATR_OC,		0x4119,	(EC_Fx + EC_x9),},
	{ERRATR_IC,		0x5101,	(EC_Px + EC_x1),},
	{ERRATR_IC,		0x5102,	(EC_Px + EC_x2),},
	{ERRATR_IC,		0x2503,	(EC_Px + EC_x4),},
	{ERRATR_IC,		0x2502,	(EC_Px + EC_x5),},
	{ERRATR_IC,		0x2500,	(EC_Px + EC_x5),},
	{ERRATR_IC,		0x1503,	(EC_Px + EC_x6),},
	{ERRATR_IC,		0x1504,	(EC_Px + EC_x6),},
	{ERRATR_IC,		0x6830,	(EC_Px + EC_x7),},
	{ERRATR_SELF,	0x6600,	(EC_Ax + EC_x0),},
	{ERRATR_SELF,	0x6602,	(EC_Ax + EC_x2),},
	{ERRATR_SELF,	0x6603,	(EC_Ax + EC_x3),},
	{ERRATR_SELF,	0x6606,	(EC_Ax + EC_x6),},
	{ERRATR_SELF,	0x6607,	(EC_Ax + EC_x7),},
	{ERRATR_SELF,	0x6608,	(EC_Ax + EC_x8),},
	{ERRATR_IC,		0x5558,	(EC_Fx + EC_xH),},		//IC 5558 FH		//[37MA A01 chata 20151222]2桁異常コードを追加
	{ERRATR_IC,		0x1510,	(EC_Fx + EC_xJ),},		//IC 1510 FJ		//[37MA A01 chata 20151222]2桁異常コードを追加
	{ERRATR_IC,		0x5501,	(EC_Fx + EC_xJ),},		//IC 5501 FJ		//[37MA A01 chata 20151222]2桁異常コードを追加
//  PAR-37MA_SW仕様連絡シート_No.10対応
//	{ERRATR_IC,		0x1515,	(EC_Fx + EC_xL),},		//IC 1515 FL		//[37MA A01 chata 20151222]2桁異常コードを追加
	{ERRATR_IC,		0x1516,	(EC_Fx + EC_xL),},		//IC 1516 FL		//mod [37MA A04 hara 20150201]異常コードを変更
	{ERRATR_IC,		0x1521,	(EC_Fx + EC_xL),},		//アプリ通信対応_202409_SEC kashiwagi
	{ERRATR_IC,		0x1522,	(EC_Fx + EC_xL),},		//アプリ通信対応_202409_SEC kashiwagi
	{0x00,			0x0000,	(EC_Ex + EC_xF),},	// 未定義コード
};
/*****************************************************************
 *	スリム異常コード変換
 *	パラメータ	：struct error *error
 *					異常コードデータ
 *	リターン	：なし
 *	２バイトコード＝００の時、４バイトコードから２バイトコードを生成
 *****************************************************************/
UB apltask_slim_errcode_conv(UB errshort, UH errlong, UB erratr)
{
	UB	n = 0;															//[A_033 NS_KOYAMA 080814]
																		//[A_033 NS_KOYAMA 080814]
	if(errshort == 0x00) {												//[A_033 NS_KOYAMA 080814]
		while(err_cnv_tbl[n].codelong != 0x0000) {						//[A_033 NS_KOYAMA 080814]
			if(err_cnv_tbl[n].codelong == errlong) {	//４桁コード一致//[A_033 NS_KOYAMA 080814]
				if((err_cnv_tbl[n].atr == ERRATR_SELF)	// 自己検出(全て)	//[A_033 NS_KOYAMA 080814]
				|| (err_cnv_tbl[n].atr == erratr)) {	// or 属性一致	//[A_033 NS_KOYAMA 080814]
					break;												//[A_033 NS_KOYAMA 080814]
				}														//[A_033 NS_KOYAMA 080814]
			}															//[A_033 NS_KOYAMA 080814]
			n++;														//[A_033 NS_KOYAMA 080814]
		}																//[A_033 NS_KOYAMA 080814]
		errshort = err_cnv_tbl[n].codeshort;							//[A_033 NS_KOYAMA 080814]
	} else {															//[A_036 NS_KOYAMA 080902]未定義コード処理追加
		if((errshort & EC_LOMASK) == EC_xUNDEF0) {						//[A_036 NS_KOYAMA 080902]
			errshort = EC_UNDEF;										//[A_036 NS_KOYAMA 080902]
		} else if(((errshort & EC_LOMASK) >= EC_xUNDEFMIN)				//[A_036 NS_KOYAMA 080902]
			   && ((errshort & EC_LOMASK) <= EC_xUNDEFMAX)) {			//[A_036 NS_KOYAMA 080902]
			errshort = EC_UNDEF;										//[A_036 NS_KOYAMA 080902]
		}else{
			// 何もしない
		}																//[A_036 NS_KOYAMA 080902]
	}																	//[A_033 NS_KOYAMA 080814]
	return(errshort);													//[A_033 NS_KOYAMA 080814]
}
#endif

/*****************************************************************
 *	BLEメンテナンス収集結果情報の更新処理(優先順位判定含む)
 *	パラメータ	：UB ucSpecifyResult_rc
 *					指定されたBLEメンテナンス収集結果
 *	リターン	：書き込み結果(APL_TRUE固定)
 *****************************************************************/
UB write_db_pri_BleMenteExeResult_fc(UB ucSpecifyResult_rc)
{
	UB ucJudgeResult_ac = BLE_MENTE_COLLECT_RESULT_NON;
	UB ucResult_Cur_ac;
	UB ucSpecifyPriority_ac;
	UB ucPriority_Cur_ac;

	/*++ 指定されたBLEメンテナンス収集結果が"未完了"でないとき */
	if(BLE_MENTE_COLLECT_RESULT_NON != ucSpecifyResult_rc)
	{
		/*++ DBからBLEメンテナンス収集結果を取得する */
		read_db_BleMenteExeResult_fc(&(ucResult_Cur_ac));
		
		/*++ 現在のBLEメンテナンス収集結果の優先順位を取得する */
		ucPriority_Cur_ac = judge_pri_BleMenteExeResult_fc(ucResult_Cur_ac);

		/*++ 指定されたBLEメンテナンス収集結果の優先順位を取得する */
		ucSpecifyPriority_ac = judge_pri_BleMenteExeResult_fc(ucSpecifyResult_rc);
		
		if( (BLE_MENTE_COLLECT_RESULT_MAX > ucPriority_Cur_ac) 
		 && (BLE_MENTE_COLLECT_RESULT_MAX > ucSpecifyPriority_ac) )
		{
			if(ucPriority_Cur_ac > ucSpecifyPriority_ac)
			{
				/*++ BLEメンテナンス収集結果に判定結果を設定する */
				write_db_BleMenteExeResult_fc(ucSpecifyResult_rc);
			}
		}
	}
	else
	{
		/*++ "未完了"を指定する */
		ucJudgeResult_ac = BLE_MENTE_COLLECT_RESULT_NON;
		
		/*++ BLEメンテナンス収集結果に判定結果を設定する */
		write_db_BleMenteExeResult_fc(ucJudgeResult_ac);
	}
	
	/*++ 取得結果を返却する */
	return(APL_TRUE);
}

/*****************************************************************
 *	BLEメンテナンス収集結果情報の優先順位判定処理
 *	パラメータ	：UB ucSpecifyResult_rc
 *					判定対象のBLEメンテナンス収集結果
 *	リターン	：優先順位
 *				  ※数字が小さい程、優先順位が高いことに注意!!
 *****************************************************************/
static UB judge_pri_BleMenteExeResult_fc(UB ucSpecifyResult_rc)
{
	UB ucCounter;

	/*++ 指定されたBLEメンテナンス収集結果の優先順位を判定する */
	/*++ 数字が若い方が優先順位が高いことに注意!!              */
	for(ucCounter = 0; ucCounter < BLE_MENTE_COLLECT_RESULT_MAX; ucCounter++)
	{
		if(ucSpecifyResult_rc == aucBleMenteExeResultMngTbl_sch[ucCounter])
		{
			break;
		}
	}

	/*++ 判定結果を返却する */
	return(ucCounter);
}

// Kawa181115 start
// 電力量収集機能追加
#ifdef MODEL_MA
/**
 * @ingroup		application
 * @brief		電力量データ4バイト→3バイト変換
 * @param[in]	four_rl 4バイトデータ
 * @return		3バイトデータ
 * @warning		なし
 * @note		なし
 * @date		2018.11.15
 * @author		MSW K.Kawamura
 */
struct nv_elecenergy aplutl_four2three_elecenergy_fv(UW four_rl){
	struct nv_elecenergy three_ax;

	/*データ有無判定*/
	if(ELECENERGY_NONE != four_rl){	/*データあり*/
		/*変換処理を実施*/
		three_ax.data_mch[0] = (UB)(four_rl & 0x000000FFL);
		three_ax.data_mch[1] = (UB)((four_rl >> 8) & 0x000000FFL);
		three_ax.data_mch[2] = (UB)((four_rl >> 16) & 0x000000FFL);
	}else{							/*データなし*/
		/*電力量データ未設定*/
		three_ax.data_mch[0] = UNDEFINED;
		three_ax.data_mch[1] = UNDEFINED;
		three_ax.data_mch[2] = UNDEFINED;
	}

	return three_ax;
}

/**
 * @ingroup		application
 * @brief		電力量データ3バイト→4バイト変換
 * @param[in]	three_rx 3バイトデータ
 * @return		4バイトデータ
 * @warning		なし
 * @note		なし
 * @date		2018.11.15
 * @author		MSW K.Kawamura
 */
UW aplutl_three2four_elecenergy_fv(struct nv_elecenergy three_rx){
	UW four_al;

	/*データ有無判定*/
	if((UNDEFINED != three_rx.data_mch[0]) ||
	   (UNDEFINED != three_rx.data_mch[1]) || 
	   (UNDEFINED != three_rx.data_mch[2])){	/*データあり*/
		/*変換処理を実施*/
		four_al = ((UW)three_rx.data_mch[2] * 0x00010000L) + ((UW)three_rx.data_mch[1] * 0x00000100L) + (UW)three_rx.data_mch[0];
	}else{										/*データなし*/
		/*電力量データ未設定*/
		four_al = ELECENERGY_NONE;
	}

	return four_al;
}

/**
 * @ingroup		application
 * @brief		電力量データアプリ→GUI変換
 * @param[in]	apl_rl アプリデータ
 * @return		GUIデータ
 * @warning		なし
 * @note		なし
 * @date		2018.11.15
 * @author		MSW K.Kawamura
 */
struct data_elecenergy aplutl_apl2gui_elecenergy_fv(UW apl_rl){
	struct data_elecenergy gui_ax;

	/*データ有無判定*/
	if(ELECENERGY_NONE != apl_rl){	/*データあり*/
		/*変換処理を実施*/
		gui_ax.integer_ml = apl_rl / 10L;
		gui_ax.decimal_mu = (UH)((apl_rl % 10L) * 10L);
	}else{							/*データなし*/
		/*電力量データ未設定*/
		gui_ax.integer_ml = ELECENERGY_INT_NONE;
		gui_ax.decimal_mu = ELECENERGY_DEC_NONE;
	}

	return gui_ax;
}

#ifdef USE_BLE
/**
 * @ingroup		application
 * @brief		電力量データGUI→BLE変換
 * @param[in]	gui_rx GUIデータ
 * @return		BLEデータ
 * @warning		なし
 * @note		なし
 * @date		2018.11.15
 * @author		MSW K.Kawamura
 */
struct nv_elecenergy aplutl_gui2ble_elecenergy_fv(struct data_elecenergy gui_rx){
	UW tmp_al;
	struct nv_elecenergy ble_ax;

	/*データ有無判定*/
	if((ELECENERGY_INT_NONE != gui_rx.integer_ml) ||
	   (ELECENERGY_DEC_NONE != gui_rx.decimal_mu)){	/*データあり*/
		/*変換処理を実施*/
		tmp_al = (gui_rx.integer_ml * 10L) + (((UW)gui_rx.decimal_mu % 100L) / 10L);
		ble_ax.data_mch[0] = (UB)(tmp_al & 0x000000FFL);
		ble_ax.data_mch[1] = (UB)((tmp_al >> 8) & 0x000000FFL);
		ble_ax.data_mch[2] = (UB)((tmp_al >> 16) & 0x000000FFL);
	}else{											/*データなし*/
		/*電力量データ未設定*/
		ble_ax.data_mch[0] = UNDEFINED;
		ble_ax.data_mch[1] = UNDEFINED;
		ble_ax.data_mch[2] = UNDEFINED;
	}

	return ble_ax;
}
#endif //USE_BLE

/**
 * @ingroup		application
 * @brief		単位時間電力量算出
 * @param[in]	now_rl 今回電力量
 * @param[in]	last_rl 前回電力量
 * @return		単位時間電力量
 * @warning		なし
 * @note		なし
 * @date		2018.11.15
 * @author		MSW K.Kawamura
 */
UW aplutl_calc_elecenergy_utime_fl(UW now_rl, UW last_rl){
	UW utime_al;

	if(now_rl >= last_rl){	/*桁あふれなし*/
		/*差分で算出*/
		utime_al = now_rl - last_rl;
	}else{					/*桁あふれあり*/
		/*最大値を加算*/
		utime_al = ELECENERGY_MAX - last_rl + now_rl + 1;
		if(ELECENERGY_VALID < utime_al){	/*有効値を超えた*/
			/*データなし*/
			utime_al = ELECENERGY_NONE;
		}
	}

	return utime_al;
}

/**
 * @ingroup		application
 * @brief		単位時間データインデックス値算出
 * @param[in]	time_rx - 指定時刻(時分)
 * @return		インデックス値
 * @warning		なし
 * @note		なし
 * @date		2018.11.15
 * @author		MSW K.Kawamura
 */
UB aplutl_calc_elecenergy_index_utime_fc(struct time time_rx){
	UB hour_ac;
	UB min_ac;
	UB index_ac;

	/*++インデックス値変換処理*/
	hour_ac = bcd2bin(time_rx.hour);
	min_ac = bcd2bin(time_rx.min);
	index_ac = (hour_ac * 2) + ((min_ac + 30) / 30) - 1;
	/*--*/
	/*範囲外判定*/
	if(UTIME_MAX < index_ac){	/*範囲外*/
		/*データ丸め込み*/
		index_ac = UTIME_MIN;
	}

	return index_ac;
}

/**
 * @ingroup		application
 * @brief		日次データ積算取得日数算出
 * @param[in]	clockday_rc - 指定日付(日)
 * @return		取得日数
 * @warning		なし
 * @note		なし
 * @date		2018.11.15
 * @author		MSW K.Kawamura
 */
UB aplutl_calc_elecenergy_num_daily_fc(UB clockday_rc){
	UB num_ac;
	UB day_ac;

	num_ac = DAILY_NUM;
	/*指定日判定*/
	if(UNDEFINED != clockday_rc){	/*指定日あり*/
		day_ac = bcd2bin(clockday_rc);
		if((DAY_MIN <= day_ac) && (DAY_MAX >= day_ac)){	/*日が正常値*/
			/*指定日の前日までの日次データを取得*/
			num_ac = day_ac - 1;
		}
	}

	return num_ac;
}

/**
 * @ingroup		application
 * @brief		単位時間データ積算電力量算出
 * @param[in]	utime_rx - 単位時間データ
 * @param[in]	date_rx - 指定日付(年月日)
 * @param[in]	time_rx - 指定時刻(時分)
 * @return		積算電力量
 * @warning		なし
 * @note		なし
 * @date		2018.11.15
 * @author		MSW K.Kawamura
 */
UW aplutl_calc_elecenergy_sum_utime_fl(struct data_utime utime_rx, struct date date_rx, struct time time_rx){
	UB loop_ac;
	UB num_ac;
	UW utime_sum_al;

	utime_sum_al = 0L;
	/*日付判定*/
	if((utime_rx.date_mx.year == date_rx.year) &&
	   (utime_rx.date_mx.month == date_rx.month) &&
	   (utime_rx.date_mx.day == date_rx.day)){	/*日付一致*/
		/*取得時刻数算出*/
		num_ac = UTIME_NUM;
		if((UNDEFINED != time_rx.hour) && (UNDEFINED != time_rx.min)){
			num_ac = aplutl_calc_elecenergy_index_utime_fc(time_rx);
		}
		/*++単位時間データ積算処理*/
		for(loop_ac = 0 ; num_ac > loop_ac ; loop_ac++){
			if(ELECENERGY_NONE != utime_rx.elecenergy_utime_mlh[loop_ac]){
				utime_sum_al = utime_sum_al + utime_rx.elecenergy_utime_mlh[loop_ac];
			}
		}
		/*--*/
	}else{										/*日付不一致*/
		/*積算データ未設定*/
		utime_sum_al = ELECENERGY_NONE;
	}

	return utime_sum_al;
}

/**
 * @ingroup		application
 * @brief		消費電力量(積算単位時間)取得
 * @param[in]	date_rx - 指定日付(年月日)
 * @param[in]	time_rx - 指定時刻(時分)
 * @return		積算電力量
 * @warning		なし
 * @note		なし
 * @date		2018.11.15
 * @author		MSW K.Kawamura
 */
UW aplutl_get_elecenergy_sum_utime_fv(struct date date_rx, struct time time_rx){
	UB month_ac;
	UB day_ac;
	UW utime_sum_al;
	struct data_utime tmp_utime_ax;

	/*++単位時間データ指定日検索*/
	/*当日分単位時間データ取得*/
	read_db_utime_day_fc(&tmp_utime_ax);
	/*単位時間データ積算電力量算出*/
	utime_sum_al = aplutl_calc_elecenergy_sum_utime_fl(tmp_utime_ax, date_rx, time_rx);
	/*当日分判定*/
	if(ELECENERGY_NONE == utime_sum_al){	/*当日分と日付不一致*/
		month_ac = bcd2bin(date_rx.month);
		day_ac = bcd2bin(date_rx.day);
		if(((DAY_MIN <= day_ac) && (DAY_MAX >= day_ac)) &&
		   ((MONTH_MIN <= month_ac) && (MONTH_MAX >= month_ac))){	/*月日が正常値*/
			/*1か月分単位時間データ取得*/
			read_db_utime_month_fc(&tmp_utime_ax, (month_ac % SIDE_NUM), (day_ac - DAILY_DIFF));
			/*単位時間データ積算電力量算出*/
			utime_sum_al = aplutl_calc_elecenergy_sum_utime_fl(tmp_utime_ax, date_rx, time_rx);
		}
	}
	/*--*/

	return utime_sum_al;
}

/**
 * @ingroup		application
 * @brief		日次データ積算電力量算出
 * @param[in]	daily_rx - 日次データ
 * @param[in]	date_rx - 指定日付(年月日)
 * @return		積算電力量
 * @warning		なし
 * @note		なし
 * @date		2018.11.15
 * @author		MSW K.Kawamura
 */
UW aplutl_calc_elecenergy_sum_daily_fl(struct data_daily daily_rx, struct date date_rx){
	UB loop_ac;
	UB num_ac;
	UW daily_sum_al;
	UW elecenergy_al;
	struct date tmp_date_ax;
	struct time tmp_time_ax;

	daily_sum_al = 0L;
	/*年月判定*/
	if((daily_rx.yearmonth_mx.year == date_rx.year) &&
	   (daily_rx.yearmonth_mx.month == date_rx.month)){	/*年月一致*/
		tmp_date_ax = date_rx;
		tmp_time_ax.hour = UNDEFINED;
		tmp_time_ax.min = UNDEFINED;
		/*取得日数算出*/
		num_ac = aplutl_calc_elecenergy_num_daily_fc(date_rx.day);
		/*++日次データ積算処理*/
		for(loop_ac = 0 ; num_ac > loop_ac ; loop_ac++){	/*取得日数分ループ*/
			elecenergy_al = daily_rx.elecenergy_daily_mlh[loop_ac];
			if(ELECENERGY_NONE == elecenergy_al){	/*日次電力量未設定*/
				tmp_date_ax.day = bin2bcd(loop_ac + 1);
				/*積算単位時間データ取得*/
				elecenergy_al = aplutl_get_elecenergy_sum_utime_fv(tmp_date_ax, tmp_time_ax);
			}
			if(ELECENERGY_NONE != elecenergy_al){	/*電力量設定済*/
				/*電力量を加算*/
				daily_sum_al = daily_sum_al + elecenergy_al;
			}
		}
		/*--*/
	}else{												/*年月不一致*/
		/*積算データ未設定*/
		daily_sum_al = ELECENERGY_NONE;
	}

	return daily_sum_al;
}

/**
 * @ingroup		application
 * @brief		単位時間データコピー
 * @param[in]	utime_rx - 単位時間データ
 * @param[out]	elecenergy_rxh - 消費電力量(単位時間)
 * @param[in]	date_rx - 指定日付(年月日)
 * @return		コピー成否
 * @warning		なし
 * @note		なし
 * @date		2018.11.15
 * @author		MSW K.Kawamura
 */
UB aplutl_copy_elecenergy_utime_fl(struct data_utime utime_rx, struct data_elecenergy elecenergy_rxh[], struct date date_rx){
	UB ret_ac;
	UB loop_ac;

	ret_ac = APL_FALSE;
	/*日付判定*/
	if((utime_rx.date_mx.year == date_rx.year) &&
	   (utime_rx.date_mx.month == date_rx.month) &&
	   (utime_rx.date_mx.day == date_rx.day)){	/*日付一致*/
		/*++単位時間データコピー処理*/
		for(loop_ac = 0 ; UTIME_NUM > loop_ac ; loop_ac++){
			elecenergy_rxh[loop_ac] = aplutl_apl2gui_elecenergy_fv(utime_rx.elecenergy_utime_mlh[loop_ac]);
		}
		/*--*/
		ret_ac = APL_TRUE;
	}

	return ret_ac;
}

/**
 * @ingroup		application
 * @brief		日次データコピー
 * @param[in]	daily_rx - 日次データ
 * @param[out]	elecenergy_rxh - 消費電力量(日次)
 * @param[in]	date_rx - 指定日付(年月日)
 * @return		コピー成否
 * @warning		なし
 * @note		なし
 * @date		2018.11.15
 * @author		MSW K.Kawamura
 */
UB aplutl_copy_elecenergy_daily_fl(struct data_daily daily_rx, struct data_elecenergy elecenergy_rxh[], struct date date_rx){
	UB ret_ac;
	UB loop_ac;
	UB num_ac;
	UW elecenergy_al;
	struct date tmp_date_ax;
	struct time tmp_time_ax;

	ret_ac = APL_FALSE;
	/*年月判定*/
	if((daily_rx.yearmonth_mx.year == date_rx.year) &&
	   (daily_rx.yearmonth_mx.month == date_rx.month)){	/*年月一致*/
		tmp_date_ax = date_rx;
		tmp_time_ax.hour = UNDEFINED;
		tmp_time_ax.min = UNDEFINED;
		/*取得日数算出*/
		num_ac = aplutl_calc_elecenergy_num_daily_fc(date_rx.day);
		/*++日次データコピー処理*/
		for(loop_ac = 0 ; num_ac > loop_ac ; loop_ac++){	/*取得日数分ループ*/
			elecenergy_al = daily_rx.elecenergy_daily_mlh[loop_ac];
			if(ELECENERGY_NONE == elecenergy_al){	/*日次電力量未設定*/
				tmp_date_ax.day = bin2bcd(loop_ac + 1);
				/*積算単位時間データ取得*/
				elecenergy_al = aplutl_get_elecenergy_sum_utime_fv(tmp_date_ax, tmp_time_ax);
			}
			elecenergy_rxh[loop_ac] = aplutl_apl2gui_elecenergy_fv(elecenergy_al);
		}
		/*--*/
		ret_ac = APL_TRUE;
	}

	return ret_ac;
}

/**
 * @ingroup		application
 * @brief		消費電力量(単位時間)取得
 * @param[out]	elecenergy_rxh - 消費電力量(単位時間)
 * @param[in]	date_rx - 指定日付(年月日)
 * @return		なし
 * @warning		なし
 * @note		なし
 * @date		2018.11.15
 * @author		MSW K.Kawamura
 */
void aplutl_get_elecenergy_utime_fv(struct data_elecenergy elecenergy_rxh[], struct date date_rx){
	UB ret_ac;
	UB loop_ac;
	UB month_ac;
	UB day_ac;
	struct data_utime tmp_utime_ax;

	for(loop_ac = 0 ; UTIME_NUM > loop_ac ; loop_ac++){
		elecenergy_rxh[loop_ac] = default_data_elecenergy_sxR;
	}
	/*++単位時間データ指定日検索*/
	/*当日分単位時間データ取得*/
	read_db_utime_day_fc(&tmp_utime_ax);
	/*単位時間データコピー*/
	ret_ac = aplutl_copy_elecenergy_utime_fl(tmp_utime_ax, elecenergy_rxh, date_rx);
	/*当日分判定*/
	if(APL_FALSE == ret_ac){	/*当日分と日付不一致*/
		month_ac = bcd2bin(date_rx.month);
		day_ac = bcd2bin(date_rx.day);
		if(((DAY_MIN <= day_ac) && (DAY_MAX >= day_ac)) &&
		   ((MONTH_MIN <= month_ac) && (MONTH_MAX >= month_ac))){	/*月日が正常値*/
			/*1か月分単位時間データ取得*/
			read_db_utime_month_fc(&tmp_utime_ax, (month_ac % SIDE_NUM), (day_ac - DAILY_DIFF));
			/*単位時間データコピー*/
			aplutl_copy_elecenergy_utime_fl(tmp_utime_ax, elecenergy_rxh, date_rx);
		}
	}
	/*--*/
}

/**
 * @ingroup		application
 * @brief		消費電力量(日次)取得
 * @param[out]	elecenergy_rxh - 消費電力量(日次)
 * @param[in]	date_rx - 指定日付(年月日)
 * @param[in]	time_rx - 指定時刻(時分)
 * @return		なし
 * @warning		なし
 * @note		なし
 * @date		2018.11.15
 * @author		MSW K.Kawamura
 */
void aplutl_get_elecenergy_daily_fv(struct data_elecenergy elecenergy_rxh[], struct date date_rx, struct time time_rx){
	UB ret_ac;
	UB loop_ac;
	UB month_ac;
	UB day_ac;
	UW utime_sum_al;
	struct data_daily tmp_daily_ax;

	for(loop_ac = 0 ; DAILY_NUM > loop_ac ; loop_ac++){
		elecenergy_rxh[loop_ac] = default_data_elecenergy_sxR;
	}
	/*++日次データ指定月検索*/
	/*当月分日次データ取得*/
	read_db_daily_current_fc(&tmp_daily_ax);
	/*日次データコピー*/
	ret_ac = aplutl_copy_elecenergy_daily_fl(tmp_daily_ax, elecenergy_rxh, date_rx);
	/*当月分判定*/
	if(APL_FALSE == ret_ac){	/*当月分と年月不一致*/
		month_ac = bcd2bin(date_rx.month);
		if((MONTH_MIN <= month_ac) && (MONTH_MAX >= month_ac)){	/*月が正常値*/
			/*年間日次データ取得*/
			read_db_daily_yearly_fc(&tmp_daily_ax, (month_ac - MONTHLY_DIFF));
			/*日次データコピー*/
			ret_ac = aplutl_copy_elecenergy_daily_fl(tmp_daily_ax, elecenergy_rxh, date_rx);
			/*年間判定*/
			if(APL_FALSE == ret_ac){	/*年間と年月不一致*/
				/*13か月前日次データ取得*/
				read_db_daily_thirteen_fc(&tmp_daily_ax);
				/*日次データコピー*/
				aplutl_copy_elecenergy_daily_fl(tmp_daily_ax, elecenergy_rxh, date_rx);
			}
		}
	}
	/*--*/
	utime_sum_al = ELECENERGY_NONE;
	if(UNDEFINED != date_rx.day){	/*指定日付あり*/
		/*積算単位時間データ取得*/
		utime_sum_al = aplutl_get_elecenergy_sum_utime_fv(date_rx, time_rx);
	}
	/*++当日日次データ書き込み*/
	if(ELECENERGY_NONE != utime_sum_al){
		day_ac = bcd2bin(date_rx.day);
		if((DAY_MIN <= day_ac) && (DAY_MAX >= day_ac)){	/*日が正常値*/
			elecenergy_rxh[day_ac - DAILY_DIFF] = aplutl_apl2gui_elecenergy_fv(utime_sum_al);
		}
	}
	/*--*/
}

/**
 * @ingroup		application
 * @brief		消費電力量(月次)取得
 * @param[out]	elecenergy_rxp - 消費電力量(月次)
 * @param[in]	date_rx - 指定日付(年月日)
 * @param[in]	time_rx - 指定時刻(時分)
 * @return		なし
 * @warning		なし
 * @note		なし
 * @date		2018.11.15
 * @author		MSW K.Kawamura
 */
void aplutl_get_elecenergy_monthly_fv(struct data_elecenergy *elecenergy_rxp, struct date date_rx, struct time time_rx){
	UB month_ac;
	UW daily_sum_al;
	UW utime_sum_al;
	UW total_al;
	struct data_daily tmp_daily_ax;

	*elecenergy_rxp = default_data_elecenergy_sxR;
	/*++日次データ指定月検索*/
	/*当月分日次データ取得*/
	read_db_daily_current_fc(&tmp_daily_ax);
	/*日次データ積算電力量算出*/
	daily_sum_al = aplutl_calc_elecenergy_sum_daily_fl(tmp_daily_ax, date_rx);
	/*当月分判定*/
	if(ELECENERGY_NONE == daily_sum_al){	/*当月分と年月不一致*/
		month_ac = bcd2bin(date_rx.month);
		if((MONTH_MIN <= month_ac) && (MONTH_MAX >= month_ac)){	/*月が正常値*/
			/*年間日次データ取得*/
			read_db_daily_yearly_fc(&tmp_daily_ax, (month_ac - MONTHLY_DIFF));
			/*日次データ積算電力量算出*/
			daily_sum_al = aplutl_calc_elecenergy_sum_daily_fl(tmp_daily_ax, date_rx);
			/*年間判定*/
			if(ELECENERGY_NONE == daily_sum_al){	/*年間と年月不一致*/
				/*13か月前日次データ取得*/
				read_db_daily_thirteen_fc(&tmp_daily_ax);
				/*日次データ積算電力量算出*/
				daily_sum_al = aplutl_calc_elecenergy_sum_daily_fl(tmp_daily_ax, date_rx);
			}
		}
	}
	/*--*/
	utime_sum_al = ELECENERGY_NONE;
	if(UNDEFINED != date_rx.day){	/*指定日付あり*/
		/*積算単位時間データ取得*/
		utime_sum_al = aplutl_get_elecenergy_sum_utime_fv(date_rx, time_rx);
	}
	/*++電力量データ合算*/
	total_al = ELECENERGY_NONE;
	if(ELECENERGY_NONE != daily_sum_al){
		if(ELECENERGY_NONE != utime_sum_al){
			total_al = daily_sum_al + utime_sum_al;
		}else{
			total_al = daily_sum_al;
		}
	}else{
		if(ELECENERGY_NONE != utime_sum_al){
			total_al = utime_sum_al;
		}
	}
	/*--*/
	/*++GUIデータ変換処理*/
	if(ELECENERGY_NONE != total_al){
		*elecenergy_rxp = aplutl_apl2gui_elecenergy_fv(total_al);
	}
	/*--*/
}

/**
 * @ingroup		application
 * @brief		消費電力量(月次)一括取得
 * @param[out]	elecenergy_rxh - 消費電力量(月次)
 * @param[in]	date_rx - 指定日付(年月日)
 * @param[in]	time_rx - 指定時刻(時分)
 * @return		なし
 * @warning		なし
 * @note		なし
 * @date		2018.11.15
 * @author		MSW K.Kawamura
 */
void aplutl_get_elecenergy_monthly_all_fv(struct data_elecenergy elecenergy_rxh[], struct date date_rx, struct time time_rx){
	UB loop_ac;
	UB year_ac;
	UB month_ac;
	struct date tmp_date_ax;
	struct time tmp_time_ax;

	year_ac = bcd2bin(date_rx.year);
	month_ac = bcd2bin(date_rx.month);
	tmp_date_ax.day = date_rx.day;
	tmp_time_ax = time_rx;
	/*++14か月分取得処理*/
	for(loop_ac = 0 ; MONTHLY_NUM_GUI > loop_ac ; loop_ac++){
		/*消費電力量(月次)取得*/
		tmp_date_ax.month = bin2bcd(month_ac);
		tmp_date_ax.year = bin2bcd(year_ac);
		aplutl_get_elecenergy_monthly_fv(&elecenergy_rxh[loop_ac], tmp_date_ax, tmp_time_ax);
		/*++年月デクリメント*/
		if(MONTH_MIN >= month_ac){
			month_ac = MONTH_MAX;
			if(YEAR_MIN >= year_ac){
				year_ac = YEAR_MAX;
			}else{
				year_ac--;
			}
		}else{
			month_ac--;
		}
		tmp_date_ax.day = UNDEFINED;
		tmp_time_ax.hour = UNDEFINED;
		tmp_time_ax.min = UNDEFINED;
		/*--*/
	}
	/*--*/
}

#ifdef USE_BLE
/**
 * @ingroup		application
 * @brief		消費電力量(BLE単位時間)取得
 * @param[out]	elecenergy_rxh - 消費電力量(BLE単位時間)
 * @param[in]	date_rx - 指定日付(年月日)
 * @return		なし
 * @warning		なし
 * @note		なし
 * @date		2018.11.15
 * @author		MSW K.Kawamura
 */
void aplutl_get_ble_elecenergy_utime_fv(struct nv_elecenergy elecenergy_rxh[], struct date date_rx){
	UB loop_ac;
	struct data_elecenergy elecenergy_axh[UTIME_NUM];

	/*消費電力量(単位時間)取得*/
	aplutl_get_elecenergy_utime_fv(elecenergy_axh, date_rx);
	/*++データ形式変換処理*/
	for(loop_ac = 0 ; UTIME_NUM > loop_ac ; loop_ac++){
		elecenergy_rxh[loop_ac] = aplutl_gui2ble_elecenergy_fv(elecenergy_axh[loop_ac]);
	}
	/*--*/
}

/**
 * @ingroup		application
 * @brief		消費電力量(BLE日次)取得
 * @param[out]	elecenergy_rxh - 消費電力量(BLE日次)
 * @param[in]	date_rx - 指定日付(年月日)
 * @param[in]	time_rx - 指定時刻(時分)
 * @return		なし
 * @warning		なし
 * @note		なし
 * @date		2018.11.15
 * @author		MSW K.Kawamura
 */
void aplutl_get_ble_elecenergy_daily_fv(struct nv_elecenergy elecenergy_rxh[], struct date date_rx, struct time time_rx){
	UB loop_ac;
	struct data_elecenergy elecenergy_axh[DAILY_NUM];

	/*消費電力量(日次)取得*/
	aplutl_get_elecenergy_daily_fv(elecenergy_axh, date_rx, time_rx);
	/*++データ形式変換処理*/
	for(loop_ac = 0 ; DAILY_NUM > loop_ac ; loop_ac++){
		elecenergy_rxh[loop_ac] = aplutl_gui2ble_elecenergy_fv(elecenergy_axh[loop_ac]);
	}
	/*--*/
}


	//↓ 国内版42MA_定期的RL78リセット機能対応_chata_20190826

/**
 * @ingroup		application
 * @brief		ble_reset_24timer設定
 * @param[out]	なし
 * @param[in]	なし
 * @return		なし
 * @warning		下記より、RAM更新後に本関数を呼ぶこと
 * @note		BLE設定および、時刻設定変更時に呼ばれる関数
 * @date		2019.08.26
 * @author		MSW chata
 */
void aplutl_set_ble_reset_24timer_fv(){

	/*++BLE_RESET_24hrタイマタイムアップ制御処理を行う*/
	struct ble_set bleset_ax;										//Bluetooth設定
	UB		quickmode_ac;											//時短モード
	UB 		closk_state_ac;											//時刻設定状態
	UW		ble_reset_24timer_al;									//24時間カウント状態

	/*++必要な情報を取得する*/
	read_db_bleset_fc(&bleset_ax);
	read_db_isclockset_fc(&closk_state_ac);
	read_db_quickmode_fc(&quickmode_ac);

	if(DB_ENABLE == bleset_ax.bleset_mc){
		if(CLOCK_INI == closk_state_ac){
			/* アラーム起動要求*/
			if(DB_DISABLE != quickmode_ac){
				/* 時短モード */
				alarm_start(ALARMID__BLERESET_24TIMER, ZITAN_BLERESET_24TIME_1MIN,OP_NONE);	// 1分=1秒に変換
			}else{
				/* 通常処理 */
				alarm_start(ALARMID__BLERESET_24TIMER, BLERESET_24TIME_1MIN,OP_NONE);
			}
			ble_reset_24timer_al = BLERESET_24TIME;
			write_db_ble_reset_24timer_fc(&ble_reset_24timer_al);
		}else{
			//処理なし
		}
	}else{
		//処理なし
	}
}

//↑ 国内版42MA_定期的RL78リセット機能対応_chata_20190826

#endif //USE_BLE
#else	// MODEL_ME
#endif	// MODEL_MA
// Kawa181115 end

// ↓国内版43MA_室内機への時刻設定対応_kameyama_20201019
/**
 * @ingroup		application
 * @brief		時刻設定要求発報判定
 * @return		戻り値 - APL_TRUE
 * @warning		なし
 * @note		なし
 * @date		2020.10.06
 * @author		Technosite
 */
#ifdef MODEL_MA
UB aplutl_clock_setting_req_jdg_fc(void){
	UB rc_state_ac;
	UB mstate_ac;
	UB service_ac;
#ifdef USE_BLE
	UB gui_id_ac;
	UB flag_ac;
#endif	// USE_BLE
	UB slimfp_ac;
	UB masterslave_ac;
	/* ↓国内版43MA_室内機への時刻設定対応_kameyama_20201125  */
	UB auto_exec_ac;
	UB day_elepwr_get_ac;
	UB zen_elepwr_get_ac;
	UB elecenergy_flg_ac;
	/* ↑国内版43MA_室内機への時刻設定対応_kameyama_20201125  */

	UB clock_setting_flg_ac;
	struct error error_ax;
	UB mente_functype_ac;																//47MA バックグラウンドで運転情報収集_MESW_Masuda_20241125


	/*リモコン状態取得*/
	read_db_rcstate_fc(&rc_state_ac);
	/*異常情報取得*/
	read_db_error_fc(&error_ax);
	/*機能選択状態取得*/
	apl_get_ac2apl_convert_mstate_fc(&mstate_ac);
	/*メンテサービス状態取得*/
	service_ac = acmm_if_get_menteservice_fc();
#ifdef USE_BLE
	flag_ac = APL_FALSE;
	/*画面ID取得*/
	// gui_id_ac = ev_get_now_gui_id_fc();		// 国内版45MA_画面ID取得方法変更_MESW_inoue_20230111
	read_db_disp_gui_id_fc(&gui_id_ac);			// 国内版45MA_画面ID取得方法変更_MESW_inoue_20230111
	/*機能選択種別を取得*/
	aplutl_get_ble_mente_functype_fv(&mente_functype_ac);								//47MA バックグラウンドで運転情報収集_MESW_Masuda_20241125
	/*Bluetooth通信中判定*/
	if((IMM_STATE_N1_G113 == gui_id_ac)	// Bluetooth通信中画面
	|| (BLE_MENTE_FUNC_TYPE_NON == mente_functype_ac)				/* 機能選択無し */	//47MA バックグラウンドで運転情報収集_MESW_Masuda_20241125
#ifdef COMPACT_MA
	|| (IMM_STATE_N1_G110 == gui_id_ac)	// 画面更新画面
#endif //COMPACT_MA
	){	/*Bluetooth通信中の場合*/
		/*Bluetooth通信中フラグ設定*/
		flag_ac = APL_TRUE;
	}
#endif	// USE_BLE
	/*機種情報取得*/
	apl_get_ac2apl_convert_slimfpconbi_fc(&slimfp_ac);
	/*主従設定取得*/
	read_db_masterslave_fc(&masterslave_ac);
	/* ↓国内版43MA_室内機への時刻設定対応_kameyama_20201125 */
	/*形名・製造No.自動収集実行状態を取得する*/
	read_db_icoc_info_auto_exec_fc(&auto_exec_ac);
	/* 前日データ取得済みフラグ取得 */
	apl_get_ac2apl_convert_day_elepwr_fg_fc(&day_elepwr_get_ac);
	/*前回データ収集済みフラグ取得*/
	apl_get_ac2apl_convert_zen_elepwr_fg_fc(&zen_elepwr_get_ac);
	/*電力量収集フラグ取得*/
	apl_get_ac2apl_convert_elecenergy_flg_fc(&elecenergy_flg_ac);
	/* ↑国内版43MA_室内機への時刻設定対応_kameyama_20201125 */
	if((RCSTATE_RUN == rc_state_ac)			// 立ち上げ中ではない
	&& ((ERROR_NONE == error_ax.err_mente) || (ERROR_MENTE == error_ax.err_mente))	// 異常中ではない
	&& (MENTE_NONE == mstate_ac)			// メンテナンス中ではない
	&& (MENTESERVICE_NONE == service_ac)	// サービス中ではない
#ifdef USE_BLE
	&& (APL_FALSE == flag_ac)				// Bluetooth通信中でない
#endif	// USE_BLE
	&& (SLIMFPCONBI_SLIM == slimfp_ac)						// 機種情報がスリムである
	&& (RC_MASTER == masterslave_ac)						// 主従設定が主リモコンである
	/* ↓国内版43MA_室内機への時刻設定対応_kameyama_20201125  */
	&& (ICOC_INFO_AUTO_EXEC_ON != auto_exec_ac)				// 形名・製造No.自動収集実行中でない
	&& (POWER_DAY_GETTING != day_elepwr_get_ac)				// 前日データ収集中でない
	&& (POWER_GETTING != zen_elepwr_get_ac)					// 前回データ収集中でない
	&& (ELECENERGY_FLG_ON != elecenergy_flg_ac)				// 電力量収集中でない
	/* ↑国内版43MA_室内機への時刻設定対応_kameyama_20201125  */
	){	/*時刻設定実行条件満足*/
		/*時刻設定フラグ取得*/
		apl_get_ac2apl_convert_clock_setting_flg_fc(&clock_setting_flg_ac);
		if(CLOCK_SETTING_FLG_OFF == clock_setting_flg_ac){	/*時刻設定中でない*/
			/*時刻設定フラグ設定*/
			clock_setting_flg_ac = CLOCK_SETTING_FLG_ON;
			apl_set_ac2apl_convert_clock_setting_flg_fc(&clock_setting_flg_ac);	// 時刻設定フラグ=設定開始
			/*時刻設定開始要求*/
			apl_req_ac2apl_convert_clock_setting_fc();
			/*調停コントローラ起動*/
			start_arbitration_controller();
		}
	}
	
	/*--*/

	return APL_TRUE;
}
#else	//MODEL_ME
#endif	//MODEL_MA
// ↑国内版43MA_室内機への時刻設定対応_kameyama_20201019

// ↓国内版44MA_CO2換気連動_MSW inoue_20211220
/**
 * @ingroup		application
 * @brief		換気連動を解除する
 * @param[out]	なし
 * @param[in]	なし
 * @return		なし
 * @warning		
 * @note		換気連動を行うことが出来なくなったときに解除する
 * @date		2021.12.20
 * @author		MSW inoue
 */
void aplutl_stop_vent_interlock_fv(void){
	/*++ (換気連動停止 ∪ 換気連動中) ⇒ 換気連動解除 への処理を行う */
	aplutl_chg_vent_interlock_state_fv(VENT_INTERLOCK_STOP, VENT_IL_REFERENCE_VENT_DB);	//46MA 遅延運転機能対応_MESW_inoue_20231208
	/*--*/
}

/**
 * @ingroup		application
 * @brief		換気連動状態を遷移する
 * @param[out]	なし
 * @param[in]	連動判定に用いる換気状態
 * @return		なし
 * @warning		
 * @note		換気連動停止∪換気連動中の状態から、換気連動停止∪換気連動中の状態へ遷移する
 * @date		2021.12.20
 * @author		MSW inoue
 */
void aplutl_chg_vent_interlock_notstop_fv(UB ref_vent_state_rc){						//46MA 遅延運転機能対応_MESW_inoue_20231208
	UB now_state_ac;
	/*++必要な情報を揃える*/
	read_db_vent_interlock_state_fc(&now_state_ac);
	/*--*/
	if (VENT_INTERLOCK_STOP != now_state_ac) {
		aplutl_chg_vent_interlock_fv(ref_vent_state_rc);								//46MA 遅延運転機能対応_MESW_inoue_20231208
	}
}

/**
 * @ingroup		application
 * @brief		換気連動状態を遷移する
 * @param[out]	なし
 * @param[in]	連動判定に用いる換気状態
 * @return		なし
 * @warning		換気連動解除状態であっても、条件を満足すれば換気連動停止∪換気連動中となってしまう。
 * 				GUI/BLEから能動的に換気連動を選択されたような場合以外は利用しないこと。
 * 				予め換気連動解除中を弾きたいなら aplutl_chg_vent_interlock_notstop_fv を利用する。
 * @note		換気連動状態にまつわる判定を行い、換気連動停止∪換気連動中の状態へ遷移する
 * @date		2021.12.20
 * @author		MSW inoue
 */
void aplutl_chg_vent_interlock_fv(UB ref_vent_state_rc){								//46MA 遅延運転機能対応_MESW_inoue_20231208
	UB next_state_ac;
	/* 遷移先の状態を判定する */
	aplutl_get_vent_interlock_state_fv(&next_state_ac, ref_vent_state_rc);				//46MA 遅延運転機能対応_MESW_inoue_20231208
	/* 状態遷移する */
	aplutl_chg_vent_interlock_state_fv(next_state_ac, ref_vent_state_rc);				//46MA 遅延運転機能対応_MESW_inoue_20231208
}

/**
 * @ingroup		application
 * @brief		換気連動状態を判定する
 * @param[out]	遷移先の換気連動状態
 * @param[in]	連動判定に用いる換気状態
 * @return		なし
 * @warning		
 * @note		
 * @date		2021.12.20
 * @author		MSW inoue
 */
void aplutl_get_vent_interlock_state_fv(UB * next_state_rc, UB ref_vent_state_rc){		//46MA 遅延運転機能対応_MESW_inoue_20231208
	UB slimfp_ac;
	struct error error_ax;
	UB rc_state_ac;
	UB mstate_ac;
	UB service_ac;
	UB	power_ac;
	struct ble_sensor_state_info ble_sensor_state_info_axp;
	struct vent vent_ax;
	UB	drv_mode_ac;
	// struct opr_lock oprlock_ax;									// 国内版45MA_CO2換気連動改善_MESW_inoue_20230116
	struct rclock rclock_ax;
	UB next_state_ac;
	struct vent_ctrl_if_switch_info ventif_sw_info_ax;										//↓46MA_換気扇IF対応_不在時設定_MESW_inoue_20231204
	UB invalid_vent_only_ope_ac;
	UB vent_interlock_active_ac;
	struct ble_senser_info ble_senser_info_ax;
	struct ble_set bleset_ax;
	UB ble_sensor_registered_ac;
	struct vent_setting_detail vent_setting_detail_ax;										//↑46MA_換気扇IF対応_不在時設定_MESW_inoue_20231204
	
	/*++必要な情報を揃える*/
	apl_get_ac2apl_convert_slimfpconbi_fc(&slimfp_ac);
	read_db_error_fc(&error_ax);
	read_db_rcstate_fc(&rc_state_ac);
	apl_get_ac2apl_convert_mstate_fc(&mstate_ac);
	service_ac = acmm_if_get_menteservice_fc();
	apl_get_ac2apl_convert_power_fc(&power_ac);
	apl_get_ac2apl_convert_ble_sensor_state_info_fc(&ble_sensor_state_info_axp);
	apl_get_ac2apl_convert_vent_fc(&vent_ax);
	apl_get_ac2apl_convert_mode_fc(&drv_mode_ac);
	// read_db_oprlock_fc(&oprlock_ax);														// 国内版45MA_CO2換気連動改善_MESW_inoue_20230116
	apl_get_ac2apl_convert_rclock_fc(&rclock_ax);
	apl_get_ac2apl_convert_vent_ctrl_if_switch_info_fc(&ventif_sw_info_ax);					//↓46MA_換気扇IF対応_不在時設定_MESW_inoue_20231204
	read_db_ble_senser_info_fc(&ble_senser_info_ax);
	read_db_bleset_fc(&bleset_ax);
	ble_sensor_registered_ac = aplutl_bd_wireless_sensor_registered_chk_fc();
	read_db_vent_setting_detail_fc(&vent_setting_detail_ax);								//↑46MA_換気扇IF対応_不在時設定_MESW_inoue_20231204
	/*--*/
	
	/*++ 指定された換気状態として扱う */													//↓46MA 遅延運転機能対応_MESW_inoue_20231208
	switch (ref_vent_state_rc) {
	case VENT_IL_REFERENCE_VENT_ON:
		/* 換気運転に変化するものとして扱う */
		vent_ax.power = VENTPOWER_ON;
		break;
	case VENT_IL_REFERENCE_VENT_OFF:
		/* 換気停止に変化するものとして扱う */
		vent_ax.power = VENTPOWER_OFF;
		break;
	case VENT_IL_REFERENCE_VENT_DB:
	default:
		/* DBから取得した値をそのまま利用する */
		break;
	}
	/*--*/																					//↑46MA 遅延運転機能対応_MESW_inoue_20231208
	
	/*++ 換気制御IF切替の状態に応じた判定を行う */											//↓46MA_換気扇IF対応_不在時設定_MESW_inoue_20231204
	if (SLIMFPCONBI_SLIM == slimfp_ac) {
		/* スリム機種は換気制御IF切替に応じた操作となる */
		switch (ventif_sw_info_ax.vent_ctrl_if_switch_RC_mc) {
		case RC_VENT_CTRL_IF_FAN_1:							// 換気制御IF有り1段
		case RC_VENT_CTRL_IF_FAN_2:							// 換気制御IF有り2段
		case RC_VENT_CTRL_IF_LOSSNAY:						// 換気制御IF有りロスナイ
			/* 換気制御IFによる接続時は換気単独操作可能 */
			invalid_vent_only_ope_ac = APL_FALSE;
			break;
		case RC_VENT_CTRL_IF_NONE:							// 換気制御IF無し
		case RC_VENT_CTRL_IF_UNDEF:							// 判定未実施状態
		default:
			/* 換気制御IF未接続・未判定・該当なし時は換気単独操作不可 */
			invalid_vent_only_ope_ac = APL_TRUE;
			break;
		}
	} else {
		/* スリム機種以外は換気単独操作可能 */
		invalid_vent_only_ope_ac = APL_FALSE;
	}
	/*--*/																					//↑46MA_換気扇IF対応_不在時設定_MESW_inoue_20231204
	
	/*++ 連動選択時の換気連動状態を判定する */
	// if (SLIMFPCONBI_SLIM == slimfp_ac) {																								//46MA_換気扇IF対応_不在時設定_MESW_inoue_20231204
	if (APL_TRUE == invalid_vent_only_ope_ac) {			/* 換気単独操作不可である */													//46MA_換気扇IF対応_不在時設定_MESW_inoue_20231204
		if(	(ERROR_ERROR != error_ax.err_mente)			/* 異常状態は異常以外である */
		&&	(RCSTATE_RUN == rc_state_ac)				/* 立上状態は通常動作中である */
		&&	(MENTE_NONE == mstate_ac)					/* 機能選択状態は通常動作中である */
		&&	(MENTESERVICE_NONE == service_ac)			/* メンテサービス状態は通常動作中である */
		// &&	(BLE_SENSOR_FOUND == ble_sensor_state_info_axp.ble_sensor_state_mc)		/* 外部センサの状態は検出中である */			//46MA_換気扇IF対応_不在時設定_MESW_inoue_20231204	
		&&	(POWER_TEST != power_ac)												/* 発停状態が試運転以外である */
		&&	(VENTPOWER_ON == vent_ax.power)											/* ロスナイ運転中である */		// 46MAより前のスリム機種はMA通信上の【ロスナイ運転】情報に依存しないが、受信解析時に変換しているのでロスナイ運転による判定可能
		// ||	((POWER_OFF == power_ac) && (UMODE_VENT == drv_mode_ac)))			/* もしくは換気単独中である */	// APL層で換気単独状態は存在しないので判定不可
		) {
			/* 全ての条件を満足するとき換気連動中となる */
			vent_interlock_active_ac = APL_TRUE;
		} else {
			/* いずれかの条件を満足しないとき換気連動停止となる */
			vent_interlock_active_ac = APL_FALSE;
		}
	} else {											/* 換気単独操作可能である */
		if(	(ERROR_ERROR != error_ax.err_mente)			/* 異常状態は異常以外である */
		&&	(RCSTATE_RUN == rc_state_ac)				/* 立上状態は通常動作中である */
		&&	(MENTE_NONE == mstate_ac)					/* 機能選択状態は通常動作中である */
		&&	(MENTESERVICE_NONE == service_ac)			/* メンテサービス状態は通常動作中である */
		// &&	(BLE_SENSOR_FOUND == ble_sensor_state_info_axp.ble_sensor_state_mc)			/* 外部センサの状態は検出中である */		//46MA_換気扇IF対応_不在時設定_MESW_inoue_20231204
		&&	(POWER_TEST != power_ac)													/* 発停状態が試運転以外である */
		&&	(VENTPOWER_ON == vent_ax.power)												/* ロスナイ運転中である */
		// &&	(	(DB_DISABLE == oprlock_ax.enable)										/* 操作ロックは無効である */				// 国内版45MA_CO2換気連動改善_MESW_inoue_20230116
		// 	||	((DB_ENABLE == oprlock_ax.enable) && (OPRUNLOCK == oprlock_ax.power)))	/* もしくは発停操作はロック解除である */		// 国内版45MA_CO2換気連動改善_MESW_inoue_20230116
		&&	(PERMIT == rclock_ax.power)													/* 発停操作禁止状態は許可中である */
		) {
			/* 全ての条件を満足するとき換気連動中となる */
			vent_interlock_active_ac = APL_TRUE;
		} else {
			/* いずれかの条件を満足しないとき換気連動停止となる */
			vent_interlock_active_ac = APL_FALSE;
		}
	}
	/*--*/
	
	/*++ 連動中となる場合、連動方法を区別する */											//↓46MA_換気扇IF対応_不在時設定_MESW_inoue_20231204
	if (APL_TRUE == vent_interlock_active_ac) {
		if(	(DB_ENABLE == ble_senser_info_ax.ble_co2_senser_enable_mc)
		&&	(DB_ENABLE == bleset_ax.bleset_mc)
		&&	(DB_ENABLE == ble_sensor_registered_ac)
		) {
			/* CO2センサによる連動制御 */
			if (BLE_SENSOR_FOUND == ble_sensor_state_info_axp.ble_sensor_state_mc) {	/* 外部センサの状態は検出中である */
				/* 全ての条件を満足するとき換気連動中となる */
				next_state_ac = VENT_INTERLOCK_ACTIVE;
			} else {
				/* いずれかの条件を満足しないとき換気連動停止となる */
				next_state_ac = VENT_INTERLOCK_PAUSE;
			}
		} else 
		if(	(DB_ENABLE == vent_setting_detail_ax.in_people_gale_enable_mb)				/* 在人強風設定が有効である */
		||	(ABSENCE_SET_DISABLE != vent_setting_detail_ax.absence_set_mc)				/* 不在時設定が無効ではない */
		) {
			/* 人感ムーブアイによる連動制御 */
			next_state_ac = VENT_INTERLOCK_ACTIVE_MOVEEYE;
		} else {
			/* 対応する連動制御がないので連動停止とする */
			/* (フェールセーフ) */
			/* (46MA時点ではGUIで連動を選択できる時点でCO2センサ連動∪人感連動に対応している) */
			next_state_ac = VENT_INTERLOCK_PAUSE;
		}
	} else {
		/* 条件を満足しないとき換気連動停止となる */
		next_state_ac = VENT_INTERLOCK_PAUSE;
	}
	/*--*/																					//↑46MA_換気扇IF対応_不在時設定_MESW_inoue_20231204
	
	/*++ 判定結果を返す */
	*next_state_rc = next_state_ac;
	/*--*/
}

/**
 * @ingroup		application
 * @brief		指定された状態への遷移を行う
 * @param[out]	なし
 * @param[in]	換気連動状態の遷移先
 * @param[in]	連動判定に用いる換気状態
 * @return		なし
 * @warning		画面描画の更新は呼び出し元にて行う
 * @note		引数で指定した状態へ遷移する
 * @date		2021.12.20
 * @author		MSW inoue
 */
static void aplutl_chg_vent_interlock_state_fv(UB next_state_rc, UB ref_vent_state_rc){		//46MA 遅延運転機能対応_MESW_inoue_20231208
	UB now_state_ac;
	
	/*++必要な情報を揃える*/
	read_db_vent_interlock_state_fc(&now_state_ac);
	/*--*/
	
	/*++状態変化するときのみ処理を行う*/
	if(next_state_rc != now_state_ac) {
		/* 換気連動状態を更新する */
		write_db_vent_interlock_state_fc(&next_state_rc);
		/* 空調管理モジュールでも状態を参照することができるようにする */
		apl_set_ac2apl_convert_vent_interlock_state_fc(&next_state_rc);
		
		switch(next_state_rc) {
		case VENT_INTERLOCK_ACTIVE:
			/*++換気連動中とする*/
			/* 換気風量の連動処理を行う */
			aplutl_chg_vent_speed_interlock_fv(DB_DISABLE, ref_vent_state_rc);				//46MA 遅延運転機能対応_MESW_inoue_20231208

			/* ムーブアイ連動による不在時制御を停止する */
			alarm_cancel(ALARMID_MEYE_IL_ABSENT);											//46MA_換気扇IF対応_不在時設定_MESW_inoue_20231204
			break;
			/*--*/
		case VENT_INTERLOCK_ACTIVE_MOVEEYE:													//↓46MA_換気扇IF対応_不在時設定_MESW_inoue_20231204
			/*++換気連動中とする*/
			aplutl_chg_vent_speed_il_meye_fv(ref_vent_state_rc);

			/* CO2濃度変化時のタイマによる風量制御を中断する */
			alarm_cancel(ALARMID_VENT_INTERLOCK);
			break;
			/*--*/																			//↑46MA_換気扇IF対応_不在時設定_MESW_inoue_20231204
		case VENT_INTERLOCK_PAUSE:
			/*++換気連動を停止する*/
			/* CO2濃度変化時のタイマによる風量制御を中断する */
			alarm_cancel(ALARMID_VENT_INTERLOCK);

			/* ムーブアイ連動による不在時制御を停止する */
			alarm_cancel(ALARMID_MEYE_IL_ABSENT);											//46MA_換気扇IF対応_不在時設定_MESW_inoue_20231204
			break;
			/*--*/
		case VENT_INTERLOCK_STOP:
			/*++換気連動を解除する*/
			/* CO2濃度変化時のタイマによる風量制御を中断する */
			alarm_cancel(ALARMID_VENT_INTERLOCK);

			/* ムーブアイ連動による不在時制御を停止する */
			alarm_cancel(ALARMID_MEYE_IL_ABSENT);											//46MA_換気扇IF対応_不在時設定_MESW_inoue_20231204
			break;
			/*--*/
		default:
			/*未定義の状態では何もしない*/
			break;
		}
		
		/* 換気描画を更新する */  // 国内版45MA_MESW_inoue_20230202
		updateinfo.vent_chg = 1;
	}
	
	/*++霜取時換気風量抑制設定の解除判定をする */
	aplutl_reset_defrost_vent_speed_reduction_control_jdg_fv();								//47MA 霜取時換気風量抑制_MESW_Masuda_20241108
	/*--*/

	/*--*/
}

/**
 * @ingroup		application
 * @brief		CO2濃度に応じて換気風量を変更する
 * @param[out]	なし
 * @param[in]	タイマを利用するかどうか
 * @param[in]	連動判定に用いる換気状態
 * @return		なし
 * @warning		
 * @note		目標CO2濃度に対する現在CO2濃度に応じた換気風量へ変更する
 * @date		2021.12.20
 * @author		MSW inoue
 */
void aplutl_chg_vent_speed_interlock_fv(UB timer_enable_rc, UB ref_vent_state_rc){			//46MA 遅延運転機能対応_MESW_inoue_20231208
	UB now_state_ac;
	struct vent vent_ax;
	UB next_vent_speed_ac;
	UB next_vent_power_ac;																	//46MA 遅延運転機能対応_MESW_inoue_20231208
	UB enable_db_update_ac;																	//46MA 遅延運転機能対応_MESW_inoue_20231208
	UB enable_ctrl_timer_ac;
	UW now_timer_cnt_al;
	UB enable_timer_start_ac;
	UB enable_chg_process_ac;
	UB quickmode_ac;
	
	/*++必要な情報を揃える*/
	read_db_vent_interlock_state_fc(&now_state_ac);
	apl_get_ac2apl_convert_vent_fc(&vent_ax);
	read_db_quickmode_fc(&quickmode_ac);
	/*--*/
	
	/*++ 指定された換気状態として扱う */													//↓46MA 遅延運転機能対応_MESW_inoue_20231208
	switch (ref_vent_state_rc) {
	case VENT_IL_REFERENCE_VENT_ON:
		/* 換気運転に変化するものとして扱う */
		next_vent_power_ac = VENTPOWER_ON;
		break;
	case VENT_IL_REFERENCE_VENT_OFF:
		/* 換気停止に変化するものとして扱う */
		next_vent_power_ac = VENTPOWER_OFF;
		break;
	case VENT_IL_REFERENCE_VENT_DB:
	default:
		/* DBから取得した値をそのまま利用する */
		next_vent_power_ac = vent_ax.power;
		break;
	}
	/*--*/																					//↑46MA 遅延運転機能対応_MESW_inoue_20231208

	next_vent_speed_ac = vent_ax.speed;

	/*++換気風量の連動は換気連動中のみ行う*/
	if(VENT_INTERLOCK_ACTIVE == now_state_ac) {
		/* 変更する換気風量と、換気風量の変更方法を取得する */
		aplutl_get_ctrl_vent_speed_fv(timer_enable_rc, &next_vent_speed_ac, &enable_ctrl_timer_ac);
		
		/* (タイマが必要となる判定結果∩既にタイマが動作している)場合以外は更新処理を行う */
		enable_chg_process_ac = DB_ENABLE;
		
		/*++タイマを管理する*/
		now_timer_cnt_al = alarm_val(ALARMID_VENT_INTERLOCK);
		enable_timer_start_ac = DB_DISABLE;
		if (DB_ENABLE == enable_ctrl_timer_ac) {
			/*++タイマによる制御を行う場合*/
			if (0 == now_timer_cnt_al) {
				/* タイマが動作していないので、タイマを開始させる */
				enable_timer_start_ac = DB_ENABLE;
			} else {
				/* タイマが動作しているので、更新処理を行わない */
				enable_chg_process_ac = DB_DISABLE;
			}
			/*--*/
		} else {
			/*++タイマによる制御を行わない場合*/
			if (0 == now_timer_cnt_al) {
				/* タイマが動作していないので、何もしない */
			} else {
				/* タイマが動作しているのでキャンセルする */
				alarm_cancel(ALARMID_VENT_INTERLOCK);
			}
			/*--*/
		}
		/*--*/

		/*++ 換気状態を更新する */															//↓46MA 遅延運転機能対応_MESW_inoue_20231208
		enable_db_update_ac = DB_DISABLE;

		/*++ 運転を変更する */
		if (vent_ax.power != next_vent_power_ac ) {
			/* 換気連動制御なので、ロスナイ運転の変化によるロスナイ風量の変更は行わない */
			vent_ax.power = next_vent_power_ac;
			enable_db_update_ac = DB_ENABLE;
		}
		/*--*/																				//↑46MA 遅延運転機能対応_MESW_inoue_20231208

		/*++ 風量を変更する */
		/* 風量が変化する場合のみ更新する */
		if(	(vent_ax.speed != next_vent_speed_ac )
		&&	(DB_ENABLE == enable_chg_process_ac)
		){
			if (DB_ENABLE == enable_timer_start_ac) {
				/* タイマによる換気操作を行う */
				if(DB_DISABLE != quickmode_ac){
					/* 時短モード */
					alarm_start(ALARMID_VENT_INTERLOCK, ZITAN_VENT_INTERLOCK_TIME, (UW)OP_NONE);
				}else{
					/* 通常処理 */
					alarm_start(ALARMID_VENT_INTERLOCK, VENT_INTERLOCK_TIME, (UW)OP_NONE);
				}
			} else {
				/* 即時換気操作を行う */
				vent_ax.speed = next_vent_speed_ac;
				// apl_set_ac2apl_convert_vent_fc(&vent_ax);								//46MA 遅延運転機能対応_MESW_inoue_20231208
				enable_db_update_ac = DB_ENABLE;											//46MA 遅延運転機能対応_MESW_inoue_20231208
			}
		}
		/*--*/

		/*++ いずれか一方でも変化するなら更新する */										//↓46MA 遅延運転機能対応_MESW_inoue_20231208
		if (DB_ENABLE == enable_db_update_ac) {
			common_util_set_vent(&vent_ax);
		}
		/*--*/
		/*--*/																				//↑46MA 遅延運転機能対応_MESW_inoue_20231208
	}
	/*--*/
}

//↓46MA 遅延運転機能対応_MESW_inoue_20231208
/**
 * @ingroup		application
 * @brief		人感連動再開時の換気制御
 * @param[out]	なし
 * @param[in]	連動判定に用いる換気状態
 * @return		なし
 * @warning		
 * @note		人感ムーブアイ連動時のロスナイ風量は在人状態の変化時に制御する
 * @date		2023.12.08
 * @author		MSW inoue
 */
void aplutl_chg_vent_speed_il_meye_fv(UB ref_vent_state_rc){
	UB vent_chg_ac = APL_FALSE;
	struct vent vent_ax;
	
	/*++必要な情報を揃える*/
	apl_get_ac2apl_convert_vent_fc(&vent_ax);
	/*--*/
	
	/*++ 指定された換気状態として扱う */
	switch (ref_vent_state_rc) {
	case VENT_IL_REFERENCE_VENT_ON:
		/*++ 換気運転に変化するものとして扱う */
		if (VENTPOWER_ON != vent_ax.power) {
			vent_ax.power = VENTPOWER_ON;
			// 風量は現在値のまま
			vent_chg_ac = APL_TRUE;
		}
		/*--*/
		break;
	case VENT_IL_REFERENCE_VENT_OFF:
		/*++ 換気停止に変化するものとして扱う */
		if (VENTPOWER_OFF != vent_ax.power) {
			vent_ax.power = VENTPOWER_OFF;
			// 連動制御時に換気停止する場合、風量は変更しない
			vent_chg_ac = APL_TRUE;
		}
		/*--*/
		break;
	case VENT_IL_REFERENCE_VENT_DB:
	default:
		/*++ DBから取得した値をそのまま利用する */
		/* 状態変化しないので何もしない */
		/*--*/
		break;
	}
	/*--*/
	
	if (APL_TRUE == vent_chg_ac) {
		common_util_set_vent(&vent_ax);
	}
}
//↑46MA 遅延運転機能対応_MESW_inoue_20231208

/**
 * @ingroup		application
 * @brief		CO2濃度に基づいて換気制御方法(換気風量とタイマ制御方法)を取得する
 * @param[out]	必要な換気風量
 * @param[out]	タイマによる制御を行うかどうか
 * @param[in]	タイマを利用するかどうか
 * @param[in]	(現在換気風量)
 * @return		なし
 * @warning		
 * @note		
 * @date		2021.12.20
 * @author		MSW inoue
 */
void aplutl_get_ctrl_vent_speed_fv(UB enable_timer_rc, UB * next_vent_speed_rcp, UB * enable_ctrl_timer_rcp){
	struct ble_senser_info ble_senser_info_ax;
	struct ble_sensor_state_info ble_sensor_state_info_ax;
	UH target_co2ppm_up_lim_au;
	UH target_co2ppm_lo_lim_au;
	UH now_co2ppm_au;
	UB next_vent_speed_ac;
	UB enable_ctrl_timer_ac;
	
	/*++必要な情報を揃える*/
	read_db_ble_senser_info_fc(&ble_senser_info_ax);
	apl_get_ac2apl_convert_ble_sensor_state_info_fc(&ble_sensor_state_info_ax);
	aplutl_get_target_co2ppm_limit_fv(ble_senser_info_ax.ble_co2ppm_target_mu, &target_co2ppm_up_lim_au, &target_co2ppm_lo_lim_au);
	/*--*/
	
	/*++閾値に対する現在CO2濃度から換気風量を決定する*/
	now_co2ppm_au = ble_sensor_state_info_ax.ble_sensor_co2ppm_mu;
	if (target_co2ppm_up_lim_au <= now_co2ppm_au) {
		/* 上限値以上の場合は風量を強にする */
		next_vent_speed_ac = VENTSPEED_HI;
		
		/* 風量を強に変更する時、タイマによる制御を行わない */
		enable_ctrl_timer_ac = DB_DISABLE;
	} else
	if (target_co2ppm_lo_lim_au > now_co2ppm_au) {
		/* 下限値未満の場合は風量を弱にする */
		next_vent_speed_ac = VENTSPEED_LOW;
		
		/* 風量を弱に変更する時、タイマを利用する場合はタイマによる制御を行う */
		if (DB_ENABLE == enable_timer_rc) {
			enable_ctrl_timer_ac = DB_ENABLE;
		} else {
			enable_ctrl_timer_ac = DB_DISABLE;
		}
	} else {
		/* 上限値以上でも下限値未満でもない場合は風量を維持する */
		next_vent_speed_ac = *next_vent_speed_rcp;
		
		/* 風量を変更しない場合、タイマによる制御を行わない */
		enable_ctrl_timer_ac = DB_DISABLE;
	}
	/*--*/
	
	/*++決定した値を返す*/
	*next_vent_speed_rcp = next_vent_speed_ac;
	*enable_ctrl_timer_rcp = enable_ctrl_timer_ac;
	/*--*/
}

/**
 * @ingroup		application
 * @brief		目標CO2濃度に応じた閾値を取得する
 * @param[out]	目標CO2濃度に対する上限値
 * @param[out]	目標CO2濃度に対する下限値
 * @param[in]	目標CO2濃度
 * @return		なし
 * @warning		
 * @note		
 * @date		2021.12.20
 * @author		MSW inoue
 */
void aplutl_get_target_co2ppm_limit_fv(UH target_co2ppm_ru, UH * target_co2ppm_up_lim_rup, UH * target_co2ppm_lo_lim_rup){
	/*++目標CO2濃度に応じた閾値を取得する*/
	switch(target_co2ppm_ru) {	// 目標CO2濃度
	case TARGET_CO2PPM_800:
		*target_co2ppm_up_lim_rup = TARGET_CO2PPM_800_UL;
		*target_co2ppm_lo_lim_rup = TARGET_CO2PPM_800_LL;
		break;
	case TARGET_CO2PPM_1000:
		*target_co2ppm_up_lim_rup = TARGET_CO2PPM_1000_UL;
		*target_co2ppm_lo_lim_rup = TARGET_CO2PPM_1000_LL;
		break;
	case TARGET_CO2PPM_1400:
		*target_co2ppm_up_lim_rup = TARGET_CO2PPM_1400_UL;
		*target_co2ppm_lo_lim_rup = TARGET_CO2PPM_1400_LL;
		break;
	default:
		break;
	}
	/*--*/
}
// ↑国内版44MA_CO2換気連動_MSW inoue_20211220

/**
 * @ingroup		application
 * @brief		BLEのメンテなし2セッションのコマンドシーケンス中断処理
 * @return		なし
 * @warning		
 * @note		本セッションによるシーケンス中はBLE通信中画面となっており、
 * @note		BLE通信中画面が中断による遷移の場合は、本関数の呼び出しが必要である。
 * @date		2022.12.22
 * @author		MSW chata
 * @note		画面の遷移先に応じて、メンテサービス状態の更新判断が必要である。
 * @note		45MA時点では、メイン画面、試運転画面からのみの遷移である。
 * @note		参考：g113_last_gui_return_fv()
 */
UB aplutl_ble_mente2_Sequence_cancel_fv(){

	UB				ucSeqType_ac;
	UB 				power_ac;
	UB 				seq_kinad_ac = APL_FALSE;
	
	//メンテなし2セッションのコマンドシーケンス実施中の場合は、シーケンス中断処理を行う。
	read_db_BleMenteExeSequenceKind_fc(&(ucSeqType_ac));
	if(BLE_MENTE_SEQ_TYPE_OPEDATA_COLLECT_NON_ANALYSIS_MENT2_SESSION == ucSeqType_ac){
		
		/*++運転状態を取得する*/
		apl_get_ac2apl_convert_power_fc(&power_ac);
		if (POWER_TEST != power_ac) {
			//①メンテセッション状態から通常中状態に遷移
			acmm_if_set_menteservice_fc(MENTESERVICE_NONE);
		}
		//②運転データ収集の中断
		apl_req_ac2apl_convert_sequence_cancel_fc();	// Kawa231013
		//③BLEメンテナンス収集シーケンス種別に"シーケンスなし"を設定する */
		write_db_BleMenteExeSequenceKind_fc(BLE_MENTE_SEQ_TYPE_NON);
		
		seq_kinad_ac = APL_TRUE;
	}
	return (seq_kinad_ac);
}

#ifdef USE_SE
//↓45MA-SE_設定温度範囲変更_MESW_Masuda_20230629
/**
 * @ingroup		application
 * @brief		設備PAC(FP、メンテナンス対応)かつジャンパー(short)か判定関数
 * 				木村工機設備PAC機種 か 木村工機以外の設備PAC機種 かの判定を行う
 * @param[out]	設備PAC機種情報
 * @return		APL_TRUE:1木村工機設備PAC機種  APL_FALSE:木村工機以外の設備PAC機種
 * @warning		
 * @note		
 * @date		2023.06.29
 * @author		MESW Masuda
 */
UB aplutl_get_SE_ID_fc(void){
	UB rtn_ac = APL_FALSE;
	UB rc_jp_ac;							// ジャンパーピン情報
	UB slimfp_ac;							// 機種情報
	struct ic_func_limit ic_func_limit_ax;	// 設定機能情報

	//ジャンパーピン情報取得
	read_db_SE_Board_ID_fc(&rc_jp_ac);
	//機種情報取得
	apl_get_ac2apl_convert_slimfpconbi_fc(&slimfp_ac);
	//室内機機能対応取得
	apl_get_ac2apl_convert_ic_func_limit_fc(&ic_func_limit_ax);

	if( (SLIMFPCONBI_FP == slimfp_ac)				//マルチ
	 && (JUMPER_BOARD_SE_ID_SPECIFIC == rc_jp_ac)	//ジャンパー 木村工機設備PAC
	 && (DB_ENABLE == ic_func_limit_ax.mente) ){	//メンテナンス対応
		rtn_ac = APL_TRUE;
	}
	
	return rtn_ac;
}
//↑45MA-SE_設定温度範囲変更_MESW_Masuda_20230629
#endif /* USE_SE */

//↓46MA 遅延運転機能対応_MESW_tsuji_20231023
/**
 * @ingroup		application
 * @brief		遅延運転開始の判定をする
 * @param[out]	発停情報
 * @return		APL_TRUE:遅延運転実施  APL_FALSE:遅延運転実施しない
 * @warning		
 * @note		
 * @date		2023.10.23
 * @author		MESW Tsuji
 */
UB aplutl_delay_unten_enable_fc(UB request_power_ac){
	struct vent_setting_detail	vent_setting_detail_ax;				//換気動作設定詳細
	struct vent_ctrl_if_switch_info vent_ctrl_if_switch_info_ax;	//換気操作
	UB quickmode_ac;												//時短モード
	UB delay_unten_enable_ac;										//遅延運転
	UB slimfp_ac;													//機種情報

	//必要な情報を取得する
	read_db_quickmode_fc(&quickmode_ac);
	read_db_vent_setting_detail_fc(&vent_setting_detail_ax);
	apl_get_ac2apl_convert_vent_ctrl_if_switch_info_fc(&vent_ctrl_if_switch_info_ax);
	apl_get_ac2apl_convert_slimfpconbi_fc(&slimfp_ac);

	//変数初期化
	delay_unten_enable_ac = APL_FALSE;

	if(POWER_TEST == request_power_ac)
	{
		//試運転中は遅延運転の処理は不要
	}
	else
	{
		// 遅延運転操作に対応している時に制御を実施する
		if((DB_DISABLE == vent_setting_detail_ax.vent24h_enable_mb)						//24時間換気=無効
		&& (RC_VENT_CTRL_OPE_B == vent_ctrl_if_switch_info_ax.vent_ctrl_if_ope_RC_mc)	//換気操作＝対応(B)
		&& (SLIMFPCONBI_SLIM == slimfp_ac)												//接続機種＝スリム
		&& (DB_ENABLE == vent_setting_detail_ax.delay_drive_enable_mb))					//遅延運転＝有効
		{
			//遅延運転を有効にする
			delay_unten_enable_ac = APL_TRUE;

			//遅延運転タイマーを開始する
			if(DB_DISABLE != quickmode_ac)
			{
				alarm_start(ALARMID_DELAY_UNTEN, ZITAN_DELAY_UNTEN_TIME, (UW)OP_NONE);	//1分=1秒に変換し30秒を設定
			}
			else
			{
				alarm_start(ALARMID_DELAY_UNTEN, DELAY_UNTEN_TIME, (UW)OP_NONE);	//30分を設定
			}
		}
	}

	return delay_unten_enable_ac;
}
//↑46MA 遅延運転機能対応_MESW_tsuji_20231023

//↓46MA 換気機器選択対応_MESW_Masuda_20231020
/**
 * @ingroup		application
 * @brief		換気機能判定関数
 * 				接続機種情報からリモコン【換気制御IF切替】、換気操作情報を判定する
 * @param[out]	リモコン【換気制御IF切替】、換気操作情報
 * @param[in]	判定した換気制御IF切替の保存アドレス
 * @param[in]	判定した換気操作の保存アドレス
 * @return		
 * @warning		
 * @note		
 * @date		2023.10.20
 * @author		MESW Masuda
 */
static void aplutl_judge_vent_ctrl_if_info_fv(UB *vent_ctrl_if_switch_rc, UB *vent_ctrl_if_ope_rc){
	struct mentenance					mentenance_ax;			// メンテナンス情報
	struct ic_func_limit				ic_func_limit_ax;		// 機能対応情報
	struct vent_setting_detail			vent_setting_detail_ax;	// 機種選択
	//struct unit_info 					unit_info_ax;			// 型番情報 <- 換気制御IF切替と換気操作 情報の判定には不要

	UB slimfp_ac;												// 機種情報
	UB vent_ctrl_if_support_ac;									// 換気制御IF対応
	UB lossnay_support_ac;										// ロスナイ接続
	UB vent_model_select_ac;									// 機種選択
	//UB moveeye_support_ac;									// 人感対応 <- 換気制御IF切替と換気操作 情報の判定には不要

	/*++ 必要な情報を取得 */
	apl_get_ac2apl_convert_slimfpconbi_fc(&slimfp_ac);			/* 接続機種 情報取得 */
	apl_get_ac2apl_convert_ic_func_limit_fc(&ic_func_limit_ax);	/* 対応機能 情報取得 */
	read_db_vent_setting_detail_fc(&vent_setting_detail_ax);	/* 機種選択 情報取得 */
	//apl_get_ac2apl_convert_unit_info_fc(&unit_info_ax);		/* 型番 情報取得 <- 換気制御IF切替と換気操作 情報の判定には不要 */
	apl_get_ac2apl_convert_mentenance_fc(&mentenance_ax);		/* メンテナンス情報取得 */
	/*--*/

	/*++ 換気制御IFに対応するのはスリム機種のみ */
	if (SLIMFPCONBI_SLIM == slimfp_ac) {
		/*++ 換気制御IF対応 判定 */
		if(DB_ENABLE == mentenance_ax.vent_ctrl_if_support_enable_mb){
			/* グループ内の全台が対応 */
			vent_ctrl_if_support_ac = DB_ENABLE;
		}else{
			/* グループ内に1台でも未対応が存在 */
			vent_ctrl_if_support_ac = DB_DISABLE;
		}
		/*--*/

		/*++ ロスナイ接続 判定 */
		if(DB_ENABLE == ic_func_limit_ax.lossnai){
			/* グループ内の全台が対応 */
			lossnay_support_ac = DB_ENABLE;
		}else{
			/* グループ内に1台でも未対応が存在 */
			lossnay_support_ac = DB_DISABLE;
		}
		/*--*/

		/*++〔換気動作設定〕機種選択 判定 */
		vent_model_select_ac = vent_setting_detail_ax.kisyu_select_mc;
		/*--*/

		/*++ 人感対応 判定 <- 換気制御IF切替と換気操作 情報の判定には不要 */
		//if((DB_DISABLE != mentenance_ax.move_eye_enable_mb)						// 人感対応ユニットが1台以上存在
		//&& (OYA_ADRS == mentenance_ax.move_eye_stop_enable0_15_mu & OYA_ADRS)	// 人感不在停止対応有効 (冷媒アドレス=0)
		//&& (0x0001 == unit_info_ax.adr)){										// 接続冷媒数=1 (冷媒アドレス=0のみ)
		//	/* 人感不在停止制御 (人感停止対応有効、接続冷媒(室外機=1)の場合) */
		//	/* 対応 */
		//	moveeye_support_ac = DB_ENABLE;
		//}else{
		//	/* 非対応 */
		//	moveeye_support_ac = DB_DISABLE;
		//}
		/*--*/
		/*--*/

		/*++ 判定 */
		/*++ リモコン換気制御IF切替 判定 */
		*vent_ctrl_if_switch_rc = aplutl_judge_vent_ctrl_if_switch_fc(vent_ctrl_if_support_ac, lossnay_support_ac, vent_model_select_ac);
		/*--*/

		/*++ 換気操作 判定 */
		*vent_ctrl_if_ope_rc = aplutl_judge_vent_ctrl_if_ope_fc(vent_ctrl_if_support_ac, lossnay_support_ac, vent_model_select_ac);
		/*--*/
		/*--*/
	}
	/*--*/

	return;
}

/**
 * @ingroup		application
 * @brief		リモコン換気制御IF切替 判定
 * @param[out]	リモコン【換気制御IF切替】を判定する
 * @param[in]	換気制御IF対応情報
 * @param[in]	ロスナイ接続情報
 * @param[in]	機種選択情報
 * @return		判定結果: ロスナイ/風量1段/風量2段/IF接続ロスナイ
 * @warning		
 * @note		入力情報より換気制御IF切替値を判定する
 * @date		2023.10.20
 * @author		MESW Masuda
 */
static UB aplutl_judge_vent_ctrl_if_switch_fc(UB vent_if_rc, UB lossnay_rc, UB model_rc){

	UB vent_ctrl_if_switch_ac = RC_VENT_CTRL_IF_NONE;

	if(DB_DISABLE==vent_if_rc){
		/* 換気制御IF対応 = 未対応存在 */
		vent_ctrl_if_switch_ac = RC_VENT_CTRL_IF_NONE;						//換気制御IF無し
	}else{
		/* 換気制御IF対応 = 全台対応 */
		if(DB_DISABLE==lossnay_rc){
			/* ロスナイ接続 = 無 */
			switch(model_rc){
				case VENT_KISYU_LOSSNAI:
					/* ロスナイ */
					vent_ctrl_if_switch_ac = RC_VENT_CTRL_IF_NONE;			//換気制御IF無し
					break;
				case VENT_KISYU_FAN_1:
					/* 風量1段 */
					vent_ctrl_if_switch_ac = RC_VENT_CTRL_IF_FAN_1;			//換気制御IF有り1段
					break;
				case VENT_KISYU_FAN_2:
					/* 風量2段 */
					vent_ctrl_if_switch_ac = RC_VENT_CTRL_IF_FAN_2;			//換気制御IF有り2段
					break;
				case VENT_KISYU_IF_CONNECT_LOSSNAI:
					/* IF接続ロスナイ */
					vent_ctrl_if_switch_ac = RC_VENT_CTRL_IF_LOSSNAY;		//換気制御IF有りロスナイ
					break;
				default:
					vent_ctrl_if_switch_ac = RC_VENT_CTRL_IF_NONE;			//換気制御IF無し
					break;
			}
		}else{
			/* ロスナイ接続 = 有 */
			switch(model_rc){
				case VENT_KISYU_LOSSNAI:
					/* ロスナイ */
					vent_ctrl_if_switch_ac = RC_VENT_CTRL_IF_NONE;			//換気制御IF無し
					break;
				case VENT_KISYU_FAN_1:
					/* 風量1段 */
					vent_ctrl_if_switch_ac = RC_VENT_CTRL_IF_FAN_1;			//換気制御IF有り1段
					break;
				case VENT_KISYU_FAN_2:
					/* 風量2段 */
					vent_ctrl_if_switch_ac = RC_VENT_CTRL_IF_FAN_2;			//換気制御IF有り2段
					break;
				case VENT_KISYU_IF_CONNECT_LOSSNAI:
					/* IF接続ロスナイ */
					vent_ctrl_if_switch_ac = RC_VENT_CTRL_IF_LOSSNAY;		//換気制御IF有りロスナイ
					break;
				default:
					vent_ctrl_if_switch_ac = RC_VENT_CTRL_IF_NONE;			//換気制御IF無し
					break;
			}
		}
	}

	return (vent_ctrl_if_switch_ac);
}

/**
 * @ingroup		application
 * @brief		換気操作 判定
 * @param[out]	換気操作を判定する
 * @param[in]	換気制御IF対応情報
 * @param[in]	ロスナイ接続情報
 * @param[in]	機種選択情報
 * @return		判定結果: 未対応/A/B
 * @warning		
 * @note		入力情報より換気操作値を判定する
 * @date		2023.10.20
 * @author		MESW Masuda
 */
static UB aplutl_judge_vent_ctrl_if_ope_fc(UB vent_if_rc, UB lossnay_rc, UB model_rc){

	UB vent_ctrl_ope_ac = RC_VENT_CTRL_OPE_NONE;

	if(DB_DISABLE==vent_if_rc){
		/* 換気制御IF対応 = 未対応存在 */
		if(DB_DISABLE==lossnay_rc){
			/* ロスナイ接続 = 無 */
			vent_ctrl_ope_ac = RC_VENT_CTRL_OPE_NONE;						//換気操作 未対応
		}else{
			/* ロスナイ接続 = 有 */
			vent_ctrl_ope_ac = RC_VENT_CTRL_OPE_A;							//換気操作 A対応
		}
	}else{
		/* 換気制御IF対応 = 全台対応 */
		if(DB_DISABLE==lossnay_rc){
			/* ロスナイ接続 = 無 */
			switch(model_rc){
				case VENT_KISYU_LOSSNAI:
					/* ロスナイ */
					vent_ctrl_ope_ac = RC_VENT_CTRL_OPE_NONE;				//換気操作 未対応
					break;
				case VENT_KISYU_FAN_1:
					/* 風量1段 */
				case VENT_KISYU_FAN_2:
					/* 風量2段 */
				case VENT_KISYU_IF_CONNECT_LOSSNAI:
					/* IF接続ロスナイ */
					vent_ctrl_ope_ac = RC_VENT_CTRL_OPE_B;					//換気操作 B対応
					break;
				default:
					vent_ctrl_ope_ac = RC_VENT_CTRL_OPE_NONE;				//換気操作 未対応
					break;
			}
		}else{
			/* ロスナイ接続 = 有 */
			switch(model_rc){
				case VENT_KISYU_LOSSNAI:
					/* ロスナイ */
					vent_ctrl_ope_ac = RC_VENT_CTRL_OPE_A;					//換気操作 A対応
					break;
				case VENT_KISYU_FAN_1:
					/* 風量1段 */
				case VENT_KISYU_FAN_2:
					/* 風量2段 */
				case VENT_KISYU_IF_CONNECT_LOSSNAI:
					/* IF接続ロスナイ */
					vent_ctrl_ope_ac = RC_VENT_CTRL_OPE_B;					//換気操作 B対応
					break;
				default:
					vent_ctrl_ope_ac = RC_VENT_CTRL_OPE_A;					//換気操作 A対応
					break;
			}
		}
	}

	return (vent_ctrl_ope_ac);
}

/**
 * @ingroup		application
 * @brief		換気動作設定詳細 デフォルト設定
 * @param[out]	換気動作設定詳細
 * @param[in]	リセットタイプ
 * @return		
 * @warning		
 * @note		接続中の換気機器が変化した場合、設定値をリセットする
 * @date		2023.10.20
 * @author		MESW Masuda
 */
void aplutl_vent_setting_detail_reset_fv(UB default_type_rc){

	struct vent_setting_detail vent_setting_detail_ax;

	/*++〔換気動作詳細設定〕の機種種別以外の全データをデフォルトに設定する*/
	/*換気動作設定詳細の値を取得する*/
	read_db_vent_setting_detail_fc(&vent_setting_detail_ax);

	/*リセット実行判定をする*/
	if(VENT_DETAIL_NOT_RESET != default_type_rc){
		/*換気動作設定画面以外からの呼び出しならリセットを実行する*/
		if(VENT_DETAIL_ALL_RESET == default_type_rc)
		{
			/*機種選択をリセットする*/
			vent_setting_detail_ax.kisyu_select_mc			= VENT_KISYU_LOSSNAI;	// 機種選択
		}else{
			/*機種選択をリセットしない*/
		}
		vent_setting_detail_ax.vent_quick_display_enable_mb	= DB_DISABLE;			// 換気急速表示
		vent_setting_detail_ax.vent24h_enable_mb			= DB_DISABLE;			// 24時間換気
		vent_setting_detail_ax.delay_drive_enable_mb		= DB_DISABLE;			// 遅延運転
		vent_setting_detail_ax.in_people_gale_enable_mb		= DB_DISABLE;			// 在人強風
		vent_setting_detail_ax.defrost_vent_speed_reduction_enable_mb	= DB_DISABLE;	// 霜取時換気風量抑制	//47MA 霜取時換気風量抑制_MESW_Masuda_20241108
		vent_setting_detail_ax.absence_set_mc				= ABSENCE_SET_DISABLE;	// 不在時設定
		/*換気動作設定詳細 設定値を書き込む*/
		write_db_vent_setting_detail_fc(&vent_setting_detail_ax);

		/*換気動作設定詳細 設定値を書き込む*/
		apl_set_ac2apl_convert_vent_setting_detail_fc(&vent_setting_detail_ax);

		/*不揮発記憶要求あり */
		updateinfo.eewrreq = APL_TRUE;
	}else{
		/* ユーザの設定を残すため、換気動作設定画面からの呼び出しはリセットしない */
	}
	
	/*++ 接続機器変化時の制御(機器の変化に起因する制御であり、設定値を残す/残さないによらない) */
	/* タイマ制御を停止する */
	alarm_cancel(ALARMID_DELAY_UNTEN);						// 遅延運転制御			//46MA 遅延運転機能対応 MESW_tsuji_20231023
	alarm_cancel(ALARMID_MEYE_IL_ABSENT);					// 不在時制御			//46MA_換気扇IF対応_不在時設定_MESW_inoue_20231204
	/*--*/
	
	/*--*/
}

/**
 * @ingroup		application
 * @brief		換気制御IF情報更新
 * @param[out]	換気制御IF切替、換気操作DB
 * @param[out]	換気制御IF切替変化状態
 * @param[in]	vent_ctrl_switch_rc:換気制御IF切替 更新値格納先
 * @param[in]	vent_ctrl_ope_rc:換気操作 更新値格納先
 * @param[in]	switch_chg_jdg_rc:換気制御IF切替 変化状態格納先
 * @param[in]	switch_diff_jdg_rc:換気制御IF切替 不一致状態格納先
 * @return		
 * @warning		
 * @note		換気制御IF切替、換気操作情報を更新
 * @note		更新した換気制御IF切替、換気操作情報から変化情報を判定
 * @date		2023.10.20
 * @author		MESW Masuda
 */
void aplutl_update_vent_ctrl_if_info_fv(UB *vent_ctrl_switch_rc, UB *vent_ctrl_ope_rc, UB *switch_chg_jdg_rc, UB *switch_diff_jdg_rc){

	UB vent_switch_RC_ac;
	UB vent_ope_RC_ac;
	UB pre_vent_switch_RC_ac;
	UB pre_vent_ope_RC_ac;
	struct vent_ctrl_if_switch_info vent_info_ax;

	/*更新前の換気制御IF切替と換気操作を取得*/
	apl_get_ac2apl_convert_vent_ctrl_if_switch_info_fc(&vent_info_ax);
	vent_switch_RC_ac = vent_info_ax.vent_ctrl_if_switch_RC_mc;
	vent_ope_RC_ac = vent_info_ax.vent_ctrl_if_ope_RC_mc;

	/* 更新前の換気制御IF切替と換気操作を保存 */
	pre_vent_switch_RC_ac = vent_info_ax.vent_ctrl_if_switch_RC_mc;
	pre_vent_ope_RC_ac = vent_info_ax.vent_ctrl_if_ope_RC_mc;

	/* 換気制御IF切替と換気操作情報を更新 */
	aplutl_judge_vent_ctrl_if_info_fv(&vent_switch_RC_ac, &vent_ope_RC_ac);

	/* 換気制御IF切替と換気操作情報の登録 */
	if((pre_vent_switch_RC_ac != vent_switch_RC_ac)
	|| (pre_vent_ope_RC_ac != vent_ope_RC_ac)){
		vent_info_ax.vent_ctrl_if_switch_RC_mc = vent_switch_RC_ac;
		vent_info_ax.vent_ctrl_if_ope_RC_mc = vent_ope_RC_ac;
		apl_set_ac2apl_convert_vent_ctrl_if_switch_info_fc(&vent_info_ax);
	}

	/* 換気制御IF切替と換気操作情報の更新値を保存 */
	*vent_ctrl_switch_rc = vent_switch_RC_ac;
	*vent_ctrl_ope_rc = vent_ope_RC_ac;

	/* 換気制御IF切替が変更したか判定 */
	if((pre_vent_switch_RC_ac != vent_switch_RC_ac)
	 && (RC_VENT_CTRL_IF_UNDEF != pre_vent_switch_RC_ac)){			// 未判定からの更新は変化としない
		*switch_chg_jdg_rc = APL_TRUE;								// 変化有
	}else{
		*switch_chg_jdg_rc = APL_FALSE;								// 変化無
	}

	/* リモコン判定の換気制御IF切替と受信した換気制御IF切替が不一致かを判定 */
	if((vent_switch_RC_ac != vent_info_ax.vent_ctrl_if_switch_IC_mc)
	 && (RC_VENT_CTRL_IF_UNDEF != pre_vent_switch_RC_ac)			// 未判定からの更新は不一致としない
	 && (RC_VENT_CTRL_IF_UNDEF != vent_info_ax.vent_ctrl_if_switch_IC_mc)){		// 換気制御IF切替が判定未実施(Bコマンド未対応)時は不一致としない	//E0不具合対応_MESW_Masuda_20240904
		*switch_diff_jdg_rc = APL_TRUE;								// 不一致
	}else{
		*switch_diff_jdg_rc = APL_FALSE;							// 一致
	}
	
	/* 換気操作が変化したか判定 */
	if(pre_vent_ope_RC_ac != vent_ope_RC_ac){
		/* B以外への変化 */
		if(RC_VENT_CTRL_OPE_B != vent_ope_RC_ac) {
			// 遅延運転は換気操作B以外非対応なので遅延運転制御停止
			alarm_cancel(ALARMID_DELAY_UNTEN);						//46MA 遅延運転機能対応_MESW_tsuji_20231023
		}

		/* 未対応への変化 */
		if(RC_VENT_CTRL_OPE_NONE == vent_ope_RC_ac) {
			// 不在時制御非対応なので制御停止
			alarm_cancel(ALARMID_MEYE_IL_ABSENT);					//46MA_換気扇IF対応_不在時設定_MESW_inoue_20231204
		}
	}
}

/**
 * @ingroup		application
 * @brief		換気制御IF情報更新
 * @param[out]	
 * @param[in]	vent_ctrl_ope_rc:換気操作
 * @param[in]	switch_chg_jdg_rc:換気制御IF切替 変化
 * @return		
 * @warning		
 * @note		換気連動中、換気制御IF切替の変化 または 換気操作が換気制御IF無しに変化した場合、換気連動を解除する
 * @date		2023.10.20
 * @author		MESW Masuda
 */
void aplutl_judge_vent_interlock_stop_fv(UB vent_ctrl_ope_rc, UB switch_chg_jdg_rc){

	/* 換気連動を解除するかを判定 */
	if((APL_TRUE == switch_chg_jdg_rc)							// 換気制御IF切替の変更判定
	|| (RC_VENT_CTRL_OPE_NONE == vent_ctrl_ope_rc)){			// 換気操作の未対応判定
		/* 換気連動を解除する */
		aplutl_stop_vent_interlock_fv();
		/* 換気描画を更新する */
		updateinfo.vent_chg = DB_ENABLE;
	}
	/*--*/
}
//↑46MA 換気機器選択対応_MESW_Masuda_20231020

//↓46MA_換気扇IF対応_24時間換気_MESW_inoue_20231121
/**
 * @ingroup		application
 * @brief		換気IF 24時間換気制御可否 判定
 * @param[out]	
 * @return		制御可能:APL_TRUE, 制御不可:APL_FALSE
 * @warning		
 * @note		COM層の判定処理と類似 group_ope_judge_ventif_24h_valid_fc
 * @note		変更する際には横展開が必要か検討すること
 * @date		2023.11.21
 * @author		MESW Inoue
 */
UB aplutl_judge_ventif_24h_valid_fc(void){
	UB valid_24h_ac = APL_FALSE;
	UB slimfp_ac;
	struct vent_ctrl_if_switch_info ventif_info_ax;
	struct vent_setting_detail		vent_setting_detail_ax;

	struct rclock rc_lock_ax;
	struct error error_ax;
	UB rc_state_ac;
	UB service_ac;
	UB mstate_ac;

	/*++ 必要な情報を取得 */
	apl_get_ac2apl_convert_slimfpconbi_fc(&slimfp_ac);
	apl_get_ac2apl_convert_vent_ctrl_if_switch_info_fc(&ventif_info_ax);
	read_db_vent_setting_detail_fc(&vent_setting_detail_ax);

	apl_get_ac2apl_convert_rclock_fc(&rc_lock_ax);
	read_db_error_fc(&error_ax);
	read_db_rcstate_fc(&rc_state_ac);
	service_ac = acmm_if_get_menteservice_fc();
	apl_get_ac2apl_convert_mstate_fc(&mstate_ac);
	/*--*/

	/*++ 24時間換気制御可否を判定する */
	if (SLIMFPCONBI_SLIM == slimfp_ac) {
		if(	(	(RC_VENT_CTRL_OPE_A == ventif_info_ax.vent_ctrl_if_ope_RC_mc)
			||	(RC_VENT_CTRL_OPE_B == ventif_info_ax.vent_ctrl_if_ope_RC_mc)
			)															// 換気操作に対応している
		&&	(DB_ENABLE == vent_setting_detail_ax.vent24h_enable_mb)		// 24時間換気設定が有効である
		){
			/*++ 24時間換気設定が有効である時の制御 */
			if(	(PERMIT == rc_lock_ax.power)							// 発停操作禁止状態は許可中である
			&&	(ERROR_ERROR != error_ax.err_mente)						// 異常状態は異常以外である
			&&	(RCSTATE_RUN == rc_state_ac)							// リモコン状態は通常動作中である
			&&	(MENTESERVICE_NONE == service_ac)						// メンテサービス状態は通常動作中である
			&&	(MENTE_NONE == mstate_ac)								// 機能選択状態は通常動作中である
			) {
				/* 全ての条件を満足するとき制御可能 */
				valid_24h_ac = APL_TRUE;
			} else {
				/* いずれかの条件を満足しないとき制御不可 */
				valid_24h_ac = APL_FALSE;
			}
			/*--*/
		} else {
			/* 24時間換気設定が有効ではない */
			valid_24h_ac = APL_FALSE;
		}
	} else {
		/* スリム以外は換気扇IFに対応しない */
		valid_24h_ac = APL_FALSE;
	}
	/*--*/

	/*++ 判定結果を返す */
	return valid_24h_ac;
	/*--*/
}
//↑46MA_換気扇IF対応_24時間換気_MESW_inoue_20231121

//↓46MA_換気扇IF対応_不在時設定_MESW_inoue_20231204
/**
 * @ingroup		application
 * @brief		人感ムーブアイ連動による不在時制御可否判定
 * @param		なし
 * @return		なし
 * @warning		なし
 * @note 		タイマ開始時とタイムアップ時条件を細分化する意味が薄いので共通の判定を行う
 * @date		2023.12.04
 * @author		MESW Inoue
 */
UB aplutl_judge_meye_interlock_absent_fv(struct vent vent_rx){
	UB result_judge_ac = APL_FALSE;
	UB slimfp_ac;
	struct vent_ctrl_if_switch_info vent_ctrl_if_switch_info_ax;
	struct ble_senser_info ble_senser_info_ax;
	struct ble_set bleset_ax;
	UB bdaddres_flg_ac;
	UB now_state_ac;
	struct vent_setting_detail vent_setting_detail_ax;

	/*++ 必要な情報を揃える */
	apl_get_ac2apl_convert_slimfpconbi_fc(&slimfp_ac);
	apl_get_ac2apl_convert_vent_ctrl_if_switch_info_fc(&vent_ctrl_if_switch_info_ax);
	read_db_ble_senser_info_fc(&ble_senser_info_ax);
	read_db_bleset_fc(&bleset_ax);
	bdaddres_flg_ac = aplutl_bd_wireless_sensor_registered_chk_fc();
	read_db_vent_interlock_state_fc(&now_state_ac);
	read_db_vent_setting_detail_fc(&vent_setting_detail_ax);
	/*--*/
	
	/*++ 不在時制御可否を判定する */
	if(	(SLIMFPCONBI_SLIM == slimfp_ac)		// スリム機種のみ換気制御IF対応
	&&	( (RC_VENT_CTRL_OPE_A == vent_ctrl_if_switch_info_ax.vent_ctrl_if_ope_RC_mc)
		||(RC_VENT_CTRL_OPE_B == vent_ctrl_if_switch_info_ax.vent_ctrl_if_ope_RC_mc)
		)									// 換気操作に対応
	) {	
		/* 不在時設定を利用可能である */
		if(	(APL_TRUE == vent_rx.interlock)							// 連動選択中である
		&&	( (DB_DISABLE == ble_senser_info_ax.ble_co2_senser_enable_mc)
			||(DB_DISABLE == bleset_ax.bleset_mc)
			||(DB_DISABLE == bdaddres_flg_ac)
			)														// CO2センサ連動不可である
		&&	(VENT_INTERLOCK_ACTIVE_MOVEEYE == now_state_ac)			// ムーブアイ連動制御中である
		&&	(VENTPOWER_ON == vent_rx.power)							// ロスナイ運転中である
		) {
			/* 人感ムーブアイによる連動制御が可能である */
			if (ABSENCE_SET_DISABLE != vent_setting_detail_ax.absence_set_mc) {
				/* 有効値が設定されている */
				result_judge_ac = APL_TRUE;
			}
		}
	}
	/*--*/

	return result_judge_ac;
}
//↑46MA_換気扇IF対応_不在時設定_MESW_inoue_20231204

//↓46MA 外部機器操作履歴対応_MESW_tsuji_20231121
/**
 * @ingroup		application
 * @brief		外部機器操作履歴保存
 * @param[out]	なし
 * @return		なし
 * @warning		なし
 * @note		なし
 * @date		2023.11.21
 * @author		MESW Tsuji
 */
void aplutl_add_external_ope_log_fv(void){

	struct external_ope_log external_ope_log_ax;
	UB rw_pos_ac;
	UB end_pos_ac;
	UB clock_state_ac;
	struct clock clock_now_time_ax;

	/*外部機器操作履歴が格納されていない位置を探す*/
	for(rw_pos_ac = 0; EXTERNAL_OPE_LOG_MAX > rw_pos_ac; rw_pos_ac++)
	{
		read_db_external_ope_log_fc(rw_pos_ac, &external_ope_log_ax);
		if(UNDEFINED == external_ope_log_ax.storage_flg_mc) {
			break;
		}
	}
	/*最大件数を超える情報は古い履歴を削除して詰める */
	if(EXTERNAL_OPE_LOG_MAX <= rw_pos_ac) {
		/*外部機器操作履歴追加不可(余裕なし)*/
		/*外部機器操作履歴全体を1つシフトする*/
		end_pos_ac = EXTERNAL_OPE_LOG_MAX - 1;
		for(rw_pos_ac = 0; end_pos_ac > rw_pos_ac; rw_pos_ac++) {
			read_db_external_ope_log_fc(rw_pos_ac+1, &external_ope_log_ax);
			write_db_external_ope_log_fc(rw_pos_ac, &external_ope_log_ax);
		}
		/* 末尾を書き込み位置とする */
		rw_pos_ac = end_pos_ac;
	}

	/*外部機器操作履歴を作成する*/
	/*時刻設定状態を読み出す*/
	read_db_isclockset_fc(&clock_state_ac);
	/*時刻設定済みかチェックする*/
	if(CLOCK_INI == clock_state_ac) {		/* 時計時刻未設定 */
		/*++異常時刻を未設定とする*/
		external_ope_log_ax.date_mx.day = UNDEFINED;
		external_ope_log_ax.date_mx.month = UNDEFINED;
		external_ope_log_ax.date_mx.year = UNDEFINED;
		external_ope_log_ax.time_mx.hour = UNDEFINED;
		external_ope_log_ax.time_mx.min = UNDEFINED;
		/*--*/
	} else {							/* 時計時刻設定済み */
		/*++現在時刻を設定する*/
		read_db_clock_fc(&clock_now_time_ax);
		external_ope_log_ax.date_mx = clock_now_time_ax.date;
		external_ope_log_ax.time_mx = clock_now_time_ax.time;
		/*--*/
	}
	/*書き込む位置を格納済みとする*/
	external_ope_log_ax.storage_flg_mc = !UNDEFINED;
	/*外部機器操作履歴へ書き込む*/
	write_db_external_ope_log_fc(rw_pos_ac, &external_ope_log_ax);
	/*不揮発記憶要求あり*/
	updateinfo.eewrreq = APL_TRUE;
}
//↑46MA 外部機器操作履歴対応_MESW_tsuji_20231121

//アプリ通信対応_異常情報管理_202409_SEC kashiwagi↓
/**
 * @ingroup		application
 * @brief		異常情報リストから異常取得
 * @param[in]	index_rc - 異常情報リストから取得するインデックス
 * @return		戻り値 - APL_TRUE
 * @note		異常情報リストの範囲内を指定された場合、指定位置の異常情報を取得する
 * 				異常情報リストの範囲外を指定された場合、最新の異常情報を取得する
 * @warning		
 * @date		2024.07.05
 * @author		Inoue
 */
UB aplutl_get_error_fc(UB index_rc, struct error *err_list_rxp)
{
	/*++ COM層の異常情報を取得する */
	apl_get_ac2apl_convert_error_fc(index_rc, err_list_rxp);
	/*--*/
	
	/*++ COM層の異常情報リストが空の場合、APL層のデータを利用する */
	/* 空調管理タスク起動前にAPL層へ格納された場合をケアするため */
	if(	(UNDEFINED == err_list_rxp->storage_mc)
	&&	(	(0 == index_rc)							// 先頭位置に格納されていないことは、リストが空であることを意味する。
		||	(ERRINFO_LATEST <= index_rc)			// 最新位置(リストの末尾)に格納されていないことは、リストが空であることを意味する。
		)
	) {
		/* APL層の異常情報を取得する */
		read_db_error_fc(err_list_rxp);
	}
	/*--*/
	
	/*++ 処理結果を返却する */
	return APL_TRUE;
	/*--*/
}
//アプリ通信対応_異常情報管理_202409_SEC kashiwagi↑

//↓47MA 霜取時換気風量抑制_MESW_Masuda_20241108
/**
 * @ingroup		application
 * @brief		霜取時換気風量抑制 制御判定処理
 * @param[out]	なし
 * @return		なし
 * @warning		なし
 * @note		運転状態変化時に、条件を満たしていれば霜取時換気風量抑制を制御する
 * @date		2024.11.08
 * @author		MESW Masuda
 */
void aplutl_set_defrost_vent_speed_reduction_control_jdg_fv(void)
{
	UB	slimfp_ac;													//機種取得
	struct rclock rclock_ax;										//発停許可/禁止
	struct error error_ac;											//異常解除／異常／メンテ
	struct vent vent_ax;											//換気運転状態、換気風量
	struct vent_setting_detail vent_setting_detail_ax;				//霜取時換気風量抑制設定
	struct vent_ctrl_if_switch_info vent_ctrl_if_switch_info_ax;	//換気操作、換気制御IF切替
	UB rc_state_ac;													//リモコン立上げ中状態
	UB mstate_ac;													//機能選択状態
	UB service_ac;													//メンテナンス状態
	UB interlock_state_ac;											//連動状態
	UB mode_ac;														//運転モード
	UB work_ac;														//DBへの書き込み用

	//霜取時換気風量抑制を制御中にする条件を取得する
	apl_get_ac2apl_convert_slimfpconbi_fc(&slimfp_ac);
	apl_get_ac2apl_convert_rclock_fc(&rclock_ax);
	read_db_error_fc(&error_ac);
	read_db_rcstate_fc(&rc_state_ac);
	service_ac = acmm_if_get_menteservice_fc();
	apl_get_ac2apl_convert_mstate_fc(&mstate_ac);
	read_db_vent_setting_detail_fc(&vent_setting_detail_ax);
	apl_get_ac2apl_convert_vent_fc(&vent_ax);
	apl_get_ac2apl_convert_vent_ctrl_if_switch_info_fc(&vent_ctrl_if_switch_info_ax);
	read_db_vent_interlock_state_fc(&interlock_state_ac);
	apl_get_ac2apl_convert_mode_fc(&mode_ac);

	//霜取時換気風量抑制の制御条件を満たしているか確認
	if(SLIMFPCONBI_SLIM == slimfp_ac){																							//スリム機種
		if(((RC_VENT_CTRL_OPE_A == vent_ctrl_if_switch_info_ax.vent_ctrl_if_ope_RC_mc)  										//換気操作 = 対応(A) or
			|| ((RC_VENT_CTRL_OPE_B == vent_ctrl_if_switch_info_ax.vent_ctrl_if_ope_RC_mc) && (PERMIT == rclock_ax.power)))			//対応(B) 且つ 操作禁止（発停）= 許可
		&& (DB_ENABLE == vent_setting_detail_ax.defrost_vent_speed_reduction_enable_mb)											//霜取時換気風量抑制 = 有効
		&& (VENT_INTERLOCK_STOP == interlock_state_ac)																			//換気設定≠連動
		&& (VENTSPEED_HI == vent_ax.speed)																						//ロスナイ風量 = 強
		&& ((UMODE_HEAT == mode_ac) || (UMODE_AUTOHEAT == mode_ac) || (UMODE_BURNHEAT == mode_ac))								//運転モード = 暖房 または 自動暖房 または 燃焼暖房
		&& (ERROR_ERROR != error_ac.err_mente)																					//異常状態≠異常
		&& (RCSTATE_RUN == rc_state_ac)																							//立上げ中 = 解除
		&& (MENTESERVICE_NONE == service_ac)																					//メンテナンス状態ではない(≠〔快適設定メニュー〕〔自動昇降設定メニュー〕〔自動清掃操作〕〔サービスメニュー〕以下画面)
		&& (MENTE_NONE == mstate_ac)																							//機能選択 = 通常
		&& (VENTPOWER_ON == vent_ax.power)																						//ロスナイ運転 = 換気運転
		&& ((RC_VENT_CTRL_IF_NONE == vent_ctrl_if_switch_info_ax.vent_ctrl_if_switch_RC_mc)										//換気制御IF無し
			|| (RC_VENT_CTRL_IF_FAN_2 == vent_ctrl_if_switch_info_ax.vent_ctrl_if_switch_RC_mc)										//換気制御IF有り2段
			|| (RC_VENT_CTRL_IF_LOSSNAY == vent_ctrl_if_switch_info_ax.vent_ctrl_if_switch_RC_mc))									//換気制御IF有りロスナイ
		){
			//条件を満たした場合、ロスナイ風量を弱で送信する
			vent_ax.speed = VENTSPEED_LOW;
			common_util_set_vent(&vent_ax);

			//霜取時換気風量抑制=制御中を登録する
			work_ac = DEFROST_VENT_SPEED_REDUCTION_CTRL;
			write_db_defrost_vent_speed_reduction_control_fc(&work_ac);
		}
	}

	//処理を終了する
	return;
}

/**
 * @ingroup		application
 * @brief		霜取時換気風量抑制 解除判定処理
 * @param[out]	なし
 * @return		なし
 * @warning		なし
 * @note		解除条件変化時に、条件を満たしていれば霜取時換気風量抑制を解除する
 * @date		2024.11.08
 * @author		MESW Masuda
 */
void aplutl_reset_defrost_vent_speed_reduction_control_jdg_fv(void)
{
	UB	slimfp_ac;													//機種取得
	struct rclock rclock_ax;										//発停許可/禁止
	struct vent vent_ax;											//換気運転状態、換気風量
	struct vent_setting_detail vent_setting_detail_ax;				//霜取時換気風量抑制設定
	struct vent_ctrl_if_switch_info vent_ctrl_if_switch_info_ax;	//換気操作、換気制御IF切替
	UB interlock_state_ac;											//連動状態
	UB mode_ac;														//運転モード
	UB vent_speed_reduction_control_ac;								//霜取時換気風量抑制状態
	UB defrost_state_ac;											//霜取中状態
	UB work_ac;														//DBへの書き込み用

	//霜取時換気風量抑制を通常にする条件を取得する
	apl_get_ac2apl_convert_slimfpconbi_fc(&slimfp_ac);
	apl_get_ac2apl_convert_rclock_fc(&rclock_ax);
	read_db_vent_setting_detail_fc(&vent_setting_detail_ax);
	apl_get_ac2apl_convert_vent_fc(&vent_ax);
	apl_get_ac2apl_convert_vent_ctrl_if_switch_info_fc(&vent_ctrl_if_switch_info_ax);
	read_db_vent_interlock_state_fc(&interlock_state_ac);
	apl_get_ac2apl_convert_mode_fc(&mode_ac);
	read_db_defrost_vent_speed_reduction_control_fc(&vent_speed_reduction_control_ac);
	apl_get_ac2apl_convert_defrost_state_fc(&defrost_state_ac);

	//霜取時換気風量抑制 解除条件を一つでも満たしているか確認
	if(SLIMFPCONBI_SLIM == slimfp_ac){																							//スリム機種
		if(DEFROST_VENT_SPEED_REDUCTION_CTRL == vent_speed_reduction_control_ac){												//現在 制御中
			if((((RC_VENT_CTRL_OPE_A != vent_ctrl_if_switch_info_ax.vent_ctrl_if_ope_RC_mc)  										//換気操作 = 対応(A)以外 かつ
				&& (RC_VENT_CTRL_OPE_B != vent_ctrl_if_switch_info_ax.vent_ctrl_if_ope_RC_mc))  										//換気操作 = 対応(B)以外 or
				|| ((RC_VENT_CTRL_OPE_B == vent_ctrl_if_switch_info_ax.vent_ctrl_if_ope_RC_mc) && (PROHIBIT == rclock_ax.power)))		//対応(B) 且つ 操作禁止（発停）= 禁止
			|| (DB_DISABLE == vent_setting_detail_ax.defrost_vent_speed_reduction_enable_mb)										//霜取時換気風量抑制 = 無効
			|| (DEFROST_STOP == defrost_state_ac)																					//霜取中停止
			|| (VENT_INTERLOCK_STOP != interlock_state_ac)																			//換気設定 = 連動
			|| (VENTSPEED_LOW != vent_ax.speed)																						//ロスナイ風量 = 強
			|| ((UMODE_HEAT != mode_ac) && (UMODE_AUTOHEAT != mode_ac) && (UMODE_BURNHEAT != mode_ac))								//運転モード = 暖房 または 自動暖房 または 燃焼暖房
			|| (VENTPOWER_OFF == vent_ax.power)																						//ロスナイ運転 = 換気停止
			|| ((RC_VENT_CTRL_IF_NONE != vent_ctrl_if_switch_info_ax.vent_ctrl_if_switch_RC_mc)										//換気制御IF無し
					&& (RC_VENT_CTRL_IF_FAN_2 != vent_ctrl_if_switch_info_ax.vent_ctrl_if_switch_RC_mc)									//換気制御IF有り2段
					&& (RC_VENT_CTRL_IF_LOSSNAY != vent_ctrl_if_switch_info_ax.vent_ctrl_if_switch_RC_mc))								//換気制御IF有りロスナイ
			){
				if(DEFROST_STOP == defrost_state_ac){
					//霜取り中→≠霜取り中 変化時,ロスナイ風量=強を送信する
					vent_ax.speed = VENTSPEED_HI;
					common_util_set_vent(&vent_ax);
				}

				//霜取時換気風量抑制=通常を登録する
				work_ac = DEFROST_VENT_SPEED_REDUCTION_NOT_CTRL;
				write_db_defrost_vent_speed_reduction_control_fc(&work_ac);
			}
		}
	}

	//処理を終了する
	return;
}
//↑47MA 霜取時換気風量抑制_MESW_Masuda_20241108

//↓47MA バックグラウンドで運転情報収集_MESW_Masuda_20241125
/**
 * @ingroup		application
 * @brief		BLEメンテナンス機能選択種別取得処理
 * @param[out]	機能選択中状態
 * @return		なし
 * @warning		なし
 * @note		なし
 * @date		2024.11.25
 * @author		MESW Masuda
 */
void aplutl_get_ble_mente_functype_fv(UB *ptFuncType_rcp)
{
	UB ucSeqType_ac;
	UB ucFuncType_ac = BLE_MENTE_FUNC_TYPE_EXIST;

	/* DBからBLEメンテナンス収集シーケンス種別を取得する */
	read_db_BleMenteExeSequenceKind_fc(&ucSeqType_ac);

	/* 機能選択中なしの判定を行う */
	if( (BLE_MENTE_SEQ_TYPE_OPEDATA_COLLECT_NON_ANALYSIS == ucSeqType_ac) 
	 || (BLE_MENTE_SEQ_TYPE_OPEDATA_CUR_COLLECT_ANALYSIS == ucSeqType_ac) 
	 || (BLE_MENTE_SEQ_TYPE_OPEDATA_PREV_COLLECT_ANALYSIS == ucSeqType_ac)
	 || (BLE_MENTE_SEQ_TYPE_OPEDATA_COLLECT_NON_ANALYSIS_MENT2_SESSION == ucSeqType_ac)
	 || (BLE_MENTE_SEQ_TYPE_REQUEST_CODE == ucSeqType_ac)
	 || (BLE_MENTE_SEQ_TYPE_FPFNCSEL_MON == ucSeqType_ac)
	 || (BLE_MENTE_SEQ_TYPE_FPFNCSEL_SET == ucSeqType_ac)
#ifdef USE_SE
	 || (BLE_MENTE_SEQ_TYPE_DMNDANLGINPT_MON == ucSeqType_ac)
	 || (BLE_MENTE_SEQ_TYPE_DMNDANLGINPT_SET == ucSeqType_ac)
#endif
	 || (BLE_MENTE_SEQ_TYPE_REQUEST_CODE_HOLD == ucSeqType_ac)
	){
		ucFuncType_ac = BLE_MENTE_FUNC_TYPE_NON;
	}
	else if(BLE_MENTE_SEQ_TYPE_NON == ucSeqType_ac)
	{
		/* シーケンス未設定もしくは、完了済み */
		ucFuncType_ac = BLE_MENTE_FUNC_TYPE_STOP;
	}
	else
	{
		/* 未知のシーケンス種別では何もしない */
	}

	/* 機能選択情報を設定する */
	*ptFuncType_rcp = ucFuncType_ac;

	/* 処理を終了する */
	return;
}
//↑47MA バックグラウンドで運転情報収集_MESW_Masuda_20241125

#ifdef USE_BLE
//↓48MA_遠隔用パスワードの仕様変更対応_Technosite_maruyama_20250923
/**
 * @ingroup		application
 * @brief		遠隔用パスワード算出
 * @param[in]	BDアドレス
 * @param[out]	MCLL認証パスワード
 * @param[out]	GW認証パスワード
 * @return		なし
 * @warning		なし
 * @note		なし
 * @date		2025.9.23
 * @author		Technosite maruyama
 */
void aplutl_calc_remote_pw_fv(UB *bdadr_rcp, UH *mcll_password_rup, UH *gw_password_rup)
{
	/*++ 変数の定義 */
	UH mcll_au;
	UH gw_au;
	/*--*/

	/*++ BDアドレス → MCLL認証パスワード */
	mcll_au = aplutl_convert_bdadr2mcll_fu(bdadr_rcp);
	*mcll_password_rup = mcll_au;
	/*--*/

	/*++ BDアドレス → GW認証パスワード */
	gw_au = aplutl_convert_bdadr2gw_fu(bdadr_rcp);
	*gw_password_rup = gw_au;
	/*--*/
}

/**
 * @ingroup		application
 * @brief		BDアドレス→MCLL認証パスワードへの変換
 * @param[in]	BDアドレス配列先頭ポインタ
 * @return		MCLL認証パスワード
 * @warning		なし
 * @note		なし
 * @date		2025.9.23
 * @author		Technosite maruyama
 */
static UH aplutl_convert_bdadr2mcll_fu(UB *bdadr_rcp)
{
	/*++ 変数の定義 */
	UB bdadr_ach[4];
	UB mcll_ach[4];
	UB loop_ac;
	UH mcll_au;
	/*--*/

	/*++ MCLLに変換する値をBDアドレスから取得 */
	bdadr_ach[0] = (bdadr_rcp[2] >> 4) & 0x0F;
	bdadr_ach[1] = bdadr_rcp[2] & 0x0F;
	bdadr_ach[2] = (bdadr_rcp[3] >> 4) & 0x0F;
	bdadr_ach[3] = bdadr_rcp[3] & 0x0F;
	/*--*/

	/*++ 変換し2byteの形に成形する */
	for(loop_ac = 0; loop_ac < 4; loop_ac++){
		mcll_ach[loop_ac] = convert_bdadr2mcll_table_sch[bdadr_ach[loop_ac]];
	}

	mcll_au = (	(((UH)(mcll_ach[0]) << 12) & 0xF000)
			+	(((UH)(mcll_ach[1]) << 8) & 0x0F00)
			+	(((UH)(mcll_ach[2]) << 4) & 0x00F0)
			+	((UH)(mcll_ach[3]) & 0x000F)	);
	/*--*/

	/*++ MCLL認証パスワードを返却 */
	return (mcll_au);
	/*--*/
}

/**
 * @ingroup		application
 * @brief		BDアドレス→GW認証パスワードへの変換
 * @param[in]	BDアドレス配列先頭ポインタ
 * @return		GW認証パスワード
 * @warning		なし
 * @note		なし
 * @date		2025.9.23
 * @author		Technosite maruyama
 */
static UH aplutl_convert_bdadr2gw_fu(UB *bdadr_rcp)
{
	/*++ 変数の定義 */
	UB bdadr_ach[4];
	UB gw_ach[4];
	UB loop_ac;
	UH gw_au;
	/*--*/

	/*++ GWに変換する値をBDアドレスから取得 */
	bdadr_ach[0] = (bdadr_rcp[4] >> 4) & 0x0F;				//48MA_GW認証パスワード参照誤り修正_Technosite_maruyama_20251016
	bdadr_ach[1] = bdadr_rcp[4] & 0x0F;						//48MA_GW認証パスワード参照誤り修正_Technosite_maruyama_20251016
	bdadr_ach[2] = (bdadr_rcp[5] >> 4) & 0x0F;				//48MA_GW認証パスワード参照誤り修正_Technosite_maruyama_20251016
	bdadr_ach[3] = bdadr_rcp[5] & 0x0F;						//48MA_GW認証パスワード参照誤り修正_Technosite_maruyama_20251016
	/*--*/

	/*++ 変換し2byteの形に成形する */
	for(loop_ac = 0; loop_ac < 4; loop_ac++){
		gw_ach[loop_ac] = convert_bdadr2gw_table_sch[bdadr_ach[loop_ac]];
	}

	gw_au = (	(((UH)(gw_ach[0]) << 12) & 0xF000)
			+	(((UH)(gw_ach[1]) << 8) & 0x0F00)
			+	(((UH)(gw_ach[2]) << 4) & 0x00F0)
			+	((UH)(gw_ach[3]) & 0x000F));
	/*--*/

	/*++ GW認証パスワードを返却 */
	return (gw_au);
	/*--*/
}
//↑48MA_遠隔用パスワードの仕様変更対応_Technosite_maruyama_20250923
#endif //USE_BLE
