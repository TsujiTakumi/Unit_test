/**
 * @file	apl_db_common.h
 * @brief	アプリケーションデータ管理ヘッダ
 * @note		
 * @note 	Dxygenコメントのつけ方[http://www.doxygen.jp/docblocks.html]
 * @date 	2015-12-08 MSW S.Sugimoto 作成
*/
 
#ifndef INCLUDE_APL_DB_COMMON_H
#define	INCLUDE_APL_DB_COMMON_H	/*!< インクルードガード	*/
#include "kernel.h"
#include "kernel_id.h"

/*
 * --------------------------------------------------------------------------
 *   Define of External Macro
 * --------------------------------------------------------------------------
 */

#define	NEWICOC_INFO				/* 室内機室外機情報新旧IF		*/

#define TEMP_TBL_MAX		41	// 設定温度･室内温度共通変換テーブルの最大値	// 35MA 131203 chata  B3コマンド対応・自動二値機能
#define TEMP_TBL_PIPING_MAX 32	// 配管温度変換テーブルの最大値

#define	WEEKLYTIMER_MAX		8			/* スケジュール数／日			*/
#define	ECOTIMER_MAX		4			/* スケジュール数／日			*/
#define	WEEK			7			/* 日数／週						*/

#define	RCNAME_LEN		16			/* リモコン名称文字列長			*/
#define	SHOPNAME_LEN		10			/* 販売店・サービス店名文字列長	*/
#define	TELNAME_LEN		13			/* TEL文字列長					*/
#define	UNITNAME_LEN		30			/* 型名文字列長					*/
#define	UNITSERIAL_LEN		15			/* 製造番号文字列長				*/
#define	NAMECODE_LEN		6			/* 型名コード文字列長			*/

#define	IC_REIBAI		4			/* １冷媒あたりの室内機台数(スリム)	*/
#define	MAX_REIBAI		16			/* 冷媒数						*/

#define	IC_MAX			64			/* 室内機台数					*/
#define	OC_MAX			16			/* 室外機台数					*/
#define	ERRLOG_MAX		36			/* 異常履歴件数					*/ //43MA_異常履歴範囲拡大対応 msw_nagasi_20200924 （件数変更：16→36）
#define	GRACE_ERRLOG_MAX	32			/* 異常猶予履歴件数				*/	// 国内版43MA_異常猶予履歴対応_inoue_20200924
#define	AUTOCHECK_MAX		4			/* 自動診断件数					*/
#define	REIBAICHECK_MAX		4			/* 冷媒診断件数					*/
#define	CO2_DAYS_MAX		7			/* CO2排出量(日次)件数			*/
#define	CO2_MONTHS_MAX		14			/* CO2排出量(月次)件数			*/
#define EXTERNAL_OPE_LOG_MAX	8		/* 外部機器操作履歴件数			*/	//46MA 外部機器操作履歴対応_MESW_tsuji_20231121

#define	DB_DISABLE		0			/* 無効							*/
#define	DB_ENABLE		1			/* 有効							*/
#define	PERMIT			0			/* 許可							*/
#define	PROHIBIT		1			/* 禁止							*/

#define	UNDEFINED		0xff		/* 未設定						*/
#define	TEMP_UNDEFINED		0x7f		/* 室温未設定					*/	//[A_019 NS_KOYAMA 080621]
#define	HUM_UNDEFINED		0x7f		/* 湿度未設定					*/	//[A_019 NS_KOYAMA 080621]

#define	CHECK_OK			0	// チェック用
#define	CHECK_NG			1	// チェック用

/****************************************************************************
 *	S/Wバージョン
 ****************************************************************************/

/****************************************************************************
 *	量産管理バージョン
 ****************************************************************************/

/****************************************************************************
 *	機種情報
 ****************************************************************************/
#define TYPE_COMPACT_MA		0			/* コンパクトMA						*/
#define TYPE_SMART_MA		1			/* スマートMA						*/
#define TYPE_SMART_ME		2			/* スマートME						*/

#define	SW_VER					0x1799								/* S/Wバージョン番号			*/
#define	SW_VER_STR				"17.99"								/* S/Wバージョン番号文字列		*/
#define	ICON_DATA_VER			0x1799								/* カラーアイコンのバージョン番号 */

#ifdef COMPACT_MA
#ifdef ROM_MAAJ			// 国内コンパクトMA
#define	MG_VER					0x0000								/* 量産管理バージョン番号		*/
#define	DST_TYPE_PREFIX			"PAC-"								/* リモコン形名接頭語			*/
#define	DST_TYPE_BODY			"SF01CR"							/* リモコン形名本体				*/
#elif defined(ROM_MAA)	// 欧州コンパクトMA
#define	MG_VER					0x0001								/* 量産管理バージョン番号		*/
#define	DST_TYPE_PREFIX			"PAR-"								/* リモコン形名接頭語			*/
#define	DST_TYPE_BODY			"CT01MA"							/* リモコン形名本体				*/
#elif defined(ROM_MAAC)	// 中国コンパクトMA
#define	MG_VER					0x0003								/* 量産管理バージョン番号		*/
#define	DST_TYPE_PREFIX			"PAR-"								/* リモコン形名接頭語			*/
#define	DST_TYPE_BODY			"CT01MAC"							/* リモコン形名本体				*/
#else	// ROM_MAAU		// 北米コンパクトMA
#define	MG_VER					0x0002								/* 量産管理バージョン番号		*/
#define	DST_TYPE_PREFIX			"PAR-"								/* リモコン形名接頭語			*/
#define	DST_TYPE_BODY			"CT01MAU"							/* リモコン形名本体				*/
#endif	// ROM_MAAJ
#elif defined (SMART_MA)
#ifdef ROM_MAAJ			// 国内スマートMA
#ifdef USE_SE
#define	MG_VER					0x0154								/* 量産管理バージョン番号		*/
#else
#define	MG_VER					0x0170								/* 量産管理バージョン番号		*/
#endif
#define	DST_TYPE_PREFIX			"PAR-"								/* リモコン形名接頭語			*/
#ifdef USE_SE
#define	DST_TYPE_BODY			"45MA-SE"							/* リモコン形名本体				*/
#else
#define	DST_TYPE_BODY			"47MA"								/* リモコン形名本体				*/
#endif	// USE_SE
#elif defined(ROM_MAA)	// 欧州スマートMA
#define	MG_VER					0x0101								/* 量産管理バージョン番号		*/
#define	DST_TYPE_PREFIX			"PAR-"								/* リモコン形名接頭語			*/
#define	DST_TYPE_BODY			"40MAA"								/* リモコン形名本体				*/
#elif defined(ROM_MAAC)	// 中国スマートMA
#define	MG_VER					0x0103								/* 量産管理バージョン番号		*/
#define	DST_TYPE_PREFIX			"PAR-"								/* リモコン形名接頭語			*/
#define	DST_TYPE_BODY			"40MAAC"							/* リモコン形名本体				*/
#else	// ROM_MAAU		// 北米スマートMA
#define	MG_VER					0x0102								/* 量産管理バージョン番号		*/
#define	DST_TYPE_PREFIX			"PAR-"								/* リモコン形名接頭語			*/
#define	DST_TYPE_BODY			"40MAAU"							/* リモコン形名本体				*/
#endif	// ROM_MAAJ
#else	// SMART_ME		// 国内スマートME
#define	MG_VER					0x0200								/* 量産管理バージョン番号		*/
#define	DST_TYPE_PREFIX			"PAR-"								/* リモコン形名接頭語			*/
#define	DST_TYPE_BODY			"F40ME"								/* リモコン形名本体				*/
#endif	// COMPACT_MA
#define	DST_TYPE				DST_TYPE_PREFIX DST_TYPE_BODY		/* リモコン形名					*/

// BLE関連
#define	BLE_DEVNAME_PREFIX		"M/R_"										/* (BLE)デバイス名称接頭語		*/
#define	BLE_DEVNAME_CNCTR		"_"											/* (BLE)デバイス名称連結文字	*/
#define BLE_IF_VER_STR			"01.08.00"									/* (BLE)I/Fバージョン			*/
#define BLE_RC_VER_STR			DST_TYPE_BODY BLE_DEVNAME_CNCTR SW_VER_STR	/* (BLE)RCバージョン			*/
#define BLE_DEV_NAME_TMP_STR	DST_TYPE_BODY "012345678901"				/* (BLE)デバイス名称仮値		*/
#define BLE_DEV_NAME_HED_STR	DST_TYPE_BODY BLE_DEVNAME_CNCTR				/* (BLE)デバイス名称前半		*/

#define DST_TYPE_BODY_LEN_MAX	7
#define BLE_DEVNAME_CNCTR_LEN	1
#define	BLE_DEVNAME_PREFIX_LEN	4
#define BLE_BD_ADDR_STR_LEN		12
#define BLE_DEV_NAME_BODY_LEN	(DST_TYPE_BODY_LEN_MAX + BLE_DEVNAME_CNCTR_LEN + BLE_BD_ADDR_STR_LEN )

#define BLE_IF_VER_SIZE			8+1
#define BLE_RC_VER_SIZE			16+1
#define BLE_DEV_NAME_SIZE		(BLE_DEVNAME_PREFIX_LEN + BLE_DEV_NAME_BODY_LEN + 1)
#define BLE_BD_ADDR_VAL_SIZE	6
#define BLE_BD_ADDR_STR_SIZE	(BLE_BD_ADDR_STR_LEN + 1)

#define BLE_DEV_NAME_STAT_DEF	0	/* デフォルト値(デバイス名称、BDアドレス共に不揮発なし) */
#define BLE_DEV_NAME_STAT_INIT	1	/* 初期値(デバイス名称不揮発なし、BDアドレス不揮発あり) */
#define BLE_DEV_NAME_STAT_SET	2	/* 設定された値(デバイス名称不揮発あり) */

/****************************************************************************
 *	リモコン状態
 ****************************************************************************/
#define	RCSTATE_INI		0			/* 初期状態						*/
#define	RCSTATE_WAIT		1			/* 立ち上げ中					*/
#define	RCSTATE_RUN		2			/* 通常動作中					*/
#define	RCSTATE_TEST		3			/* 出荷検査						*/

/****************************************************************************
 *	メンテナンス状態
 ****************************************************************************/
#define	MENTE_NONE		0			/* 通常動作中					*/
#define	MENTE_SET		1			/* メンテ中						*/

/****************************************************************************
 *	メンテサービス状態
 ****************************************************************************/
#define	MENTESERVICE_NONE	0		/* 通常動作中					*/	//[A_024 NS_KOYAMA 080722]
#define	MENTESERVICE_SET	1		/* メンテサービス中				*/	//[A_024 NS_KOYAMA 080722]

/****************************************************************************
 *	運転／停止
 ****************************************************************************/
#define	POWER_OFF		0			/* 停止							*/
#define	POWER_ON		1			/* 運転							*/
#define	POWER_TEST		3			/* 試運転						*/
#define POWER_DELAY_UNTEN 4			/* 遅延運転						*/	//46MA 遅延運転機能対応 MESW_tsuji_20231023

/****************************************************************************
 *	換気
 ****************************************************************************/
/****************************************************************************
 *	前回換気
 ****************************************************************************/
#define	VENTPOWER_ON		1			/*!< 換気運転						*/
#define	VENTPOWER_OFF		0			/*!< 換気停止						*/

#define	VENTSPEED_HI		1			/*!< 換気強						*/
#define	VENTSPEED_LOW		0			/*!< 換気弱						*/

#define VENTINTERLOCK_ON	1			/* 換気連動選択中				*/	// 国内版44MA_CO2換気連動_MSW inoue_20211220
#define VENTINTERLOCK_OFF	0			/* 換気連動非選択中				*/	// 国内版44MA_CO2換気連動_MSW inoue_20211220

/****************************************************************************
 *	換気連動状態 // 国内版44MA_CO2換気連動_MSW inoue_20211220
 ****************************************************************************/
#define VENT_INTERLOCK_STOP				0	/* 換気連動解除					*/
#define VENT_INTERLOCK_PAUSE			1	/* 換気連動停止					*/
#define VENT_INTERLOCK_ACTIVE			2	/* 換気連動中(CO2センサ)		*/	//46MA_換気扇IF対応_不在時設定_MESW_inoue_20231204
#define VENT_INTERLOCK_ACTIVE_MOVEEYE	3	/* 換気連動中(人感ムーブアイ)	*/	//46MA_換気扇IF対応_不在時設定_MESW_inoue_20231204

/****************************************************************************
 *	換気連動判定に用いる換気状態 //46MA 遅延運転機能対応_MESW_inoue_20231208
 ****************************************************************************/
#define VENT_IL_REFERENCE_VENT_ON		1				/* 換気運転=運転への変化として扱う			*/
#define VENT_IL_REFERENCE_VENT_OFF		0				/* 換気運転=停止への変化として扱う			*/
#define VENT_IL_REFERENCE_VENT_DB		0xFF			/* DBに保存された状態を参照する				*/

/****************************************************************************
 *	目標CO2濃度 // 国内版44MA_CO2換気連動_MSW inoue_20211220
 ****************************************************************************/
#define TARGET_CO2PPM_800		800			/* 目標CO2濃度800ppm			*/
#define TARGET_CO2PPM_1000		1000		/* 目標CO2濃度1000ppm			*/
#define TARGET_CO2PPM_1400		1400		/* 目標CO2濃度1400ppm			*/

/****************************************************************************
 *	目標CO2濃度閾値 // 国内版44MA_CO2換気連動_MSW inoue_20211220
 ****************************************************************************/
#define TARGET_CO2PPM_800_UL		(TARGET_CO2PPM_800 - 200)			/* 目標CO2濃度800ppm 上限値				*/
#define TARGET_CO2PPM_800_LL		(TARGET_CO2PPM_800 - 250)			/* 目標CO2濃度800ppm 下限値				*/
#define TARGET_CO2PPM_1000_UL		(TARGET_CO2PPM_1000 - 200)			/* 目標CO2濃度1000ppm 上限値			*/
#define TARGET_CO2PPM_1000_LL		(TARGET_CO2PPM_1000 - 250)			/* 目標CO2濃度1000ppm 下限値			*/
#define TARGET_CO2PPM_1400_UL		(TARGET_CO2PPM_1400 - 100)			/* 目標CO2濃度1400ppm 上限値			*/
#define TARGET_CO2PPM_1400_LL		(TARGET_CO2PPM_1400 - 150)			/* 目標CO2濃度1400ppm 下限値			*/

/****************************************************************************
 *	運転モード
 ****************************************************************************/
#define	UMODE_FAN		0			/* 送風							*/
#define	UMODE_COOL		1			/* 冷房							*/
#define	UMODE_HEAT		2			/* 暖房							*/
#define	UMODE_AUTOCOOL		3			/* 自動冷房						*/
#define	UMODE_AUTOHEAT		4			/* 自動暖房						*/
#define	UMODE_AUTO		0x13			/* 自動							*/
#define	UMODE_DRY		6			/* ドライ						*/
#define	UMODE_BURNHEAT		7			/* 燃焼暖房						*/
#define	UMODE_VENT		8			/* 換気単独						*/
#define	UMODE_SETBACK		0x0e		/* セットバック					*/
#define	UMODE_AUTO_2		0x0f		/* 自動(二値)					*/

/****************************************************************************
 *	運転状態
 ****************************************************************************/
#define	UNITSTATE_NORMAL	0			/* 通常							*/
#define	UNITSTATE_HEATSTBY	1			/* 暖房準備中					*/
#define	UNITSTATE_DEFROST	2			/* 霜取り中						*/
#define	UNITSTATE_MULTISTBY	4			/* マルチ待機中					*/
#define	UNITSTATE_NORMAL_C	5			/* 通常(冷房)					*/
#define	UNITSTATE_NORMAL_H	6			/* 通常(暖房)					*/
#define	UNITSTATE_COMPOFF	7			/* CompOFF						*/ //[A_018 NS_KOYAMA 080620]
#define	UNITSTATE_PROHIBIT	8			/* 禁止中						*/ 

/****************************************************************************			// 35MA 131203 chata B3コマンド対応・省エネ表示対応
 *	能力セーブ状態																		// 35MA 131203 chata B3コマンド対応・省エネ表示対応
 ****************************************************************************/			// 35MA 131203 chata B3コマンド対応・省エネ表示対応
#define	ECO_OFF				0			/* 通常(セーブなし)				*/				// 35MA 131203 chata B3コマンド対応・省エネ表示対応
#define	ECO_ON				1			/* 能力セーブ運転中				*/		

/****************************************************************************
 *	燃焼中
 ****************************************************************************/
#define	BURN_ON			1			/* 燃焼中						*/
#define	BURN_OFF		0			/* 燃焼解除						*/

/****************************************************************************
 *	設定室温
 ****************************************************************************/

/****************************************************************************
 *	設定湿度
 ****************************************************************************/

/****************************************************************************
 *	風速
 ****************************************************************************/
#define	FANSPEED_1		0			/* 風速１段						*/
#define	FANSPEED_2		1			/* 風速２段						*/
#define	FANSPEED_3		2			/* 風速３段						*/
#define	FANSPEED_4		3			/* 風速４段						*/
#define	FANSPEED_5		7			/* 風速５段						*/
#define	FANSPEED_AUTO		4			/* 風速自動						*/

/****************************************************************************
 *	上下風向
 ****************************************************************************/
#define	VANE_100		0			/* 下吹き１００％				*/
#define	VANE_80			1			/* 下吹き８０％					*/
#define	VANE_60			2			/* 下吹き６０％					*/
#define	VANE_20			5			/* 下吹き２０％					*/
#define	VANE_FLAT		3			/* 水平吹き						*/
#define	VANE_AUTO		6			/* 自動							*/
#define	VANE_SWING		7			/* スイング						*/
#define	VANE_CODEREQ		4			/* コード要求					*/

/****************************************************************************
 *	左右風向																		// [40MA国内1st MSW Miyazaki]
 ****************************************************************************/
#define	LR_FUKOU_CENTER		0			/* 正面						*/
#define	LR_FUKOU_SWING		1			/* スイング					*/
#define	LR_FUKOU_AUTO		2			/* 自動						*/
#define	LR_FUKOU_LEFT		3			/* 左						*/
#define	LR_FUKOU_LEFTMID	4			/* 左中間					*/
#define	LR_FUKOU_RIGHTMID	5			/* 右中間					*/
#define	LR_FUKOU_RIGHT		6			/* 右						*/

/****************************************************************************
 *	人感一括																		// [40MA国内1st MSW Miyazaki]
 ****************************************************************************/
#define	MOVEEYE_IKKATSU_OFF			0		/* 制御なし					*/
#define	MOVEEYE_IKKATSU_KAZEATE		1		/* 風あて					*/
#define	MOVEEYE_IKKATSU_KAZEYOKE	2		/* 風よけ					*/
#define	MOVEEYE_IKKATSU_MURANASI	3		/* ムラ無し					*/
#define	MOVEEYE_IKKATSU_AUTO		4		/* 自動						*/			//[42MA N.Ono 190801]
#define	MOVEEYE_IKKATSU_ATEYOKEAUTO	5		/* アテヨケ自動				*/			//[国内版44MA 新4カセ対応 SEC M.TOJO 20210929]

/****************************************************************************
 *	ベーン１時間有効無効
 ****************************************************************************/
#define	VANE1H_ON		1			/* ベーン１時間有効				*/
#define	VANE1H_OFF		0			/* ベーン１時間無効				*/

/****************************************************************************
 *	フィルターチェック
 ****************************************************************************/
#define	FILTER_CHECK		1			/* フィルターチェック			*/
#define	FILTER_OK		0			/* フィルター通常				*/

/****************************************************************************
 *	ルーバー
 ****************************************************************************/
#define	LOUVER_ON		1			/* ルーバー運転					*/
#define	LOUVER_OFF		0			/* ルーバー停止					*/

/****************************************************************************
 *	左右風向
 ****************************************************************************/
/*!< @todo 仕様決定後に修正 >*/
#define	LR_SWING_VANE_FRONT		0			/*!< 正面						*/
#define	LR_SWING_VANE_SWING		1			/*!< スイング					*/
#define	LR_SWING_VANE_AUTO		2			/*!< 自動						*/

/****************************************************************************
 *	室温
 ****************************************************************************/

/****************************************************************************
 *	快速冷暖運転
 ****************************************************************************/
#define	QUICKSTART_ON		1			/* 快速冷暖運転					*/
#define	QUICKSTART_OFF		0			/* 快速冷暖停止					*/

/****************************************************************************
 *	２４時間換気運転
 ****************************************************************************/
#define	VENT24H_ON		1			/* ２４時間換気運転				*/
#define	VENT24H_OFF		0			/* ２４時間換気停止				*/
/****************************************************************************
 *	異常
 ****************************************************************************/

/*異常状態*/
#define	ERROR_NONE		0			/* 正常							*/
#define	ERROR_ERROR		1			/* 異常							*/
#define	ERROR_MENTE		2			/* メンテ						*/

/*ユニット属性*/
#define	ERRATR_IC		0			/* ＩＣ							*/
#define	ERRATR_OC		1			/* ＯＣ							*/
#define	ERRATR_FU		2			/* ＦＵ							*/
#define	ERRATR_LC		3			/* ＬＣ							*/
#define	ERRATR_BC		4			/* ＢＣ							*/
#define	ERRATR_SC		5			/* ＳＣ							*/
#define	ERRATR_UC		6			/* ＵＣ							*/
#define	ERRATR_RC		7			/* ＲＣ							*/
#define	ERRATR_OS		8			/* ＯＳ							*/
#define	ERRATR_BS		9			/* ＢＳ							*/
#define	ERRATR_NONE		0xfe		/* 属性無し						*/
#define ERRATR_SELF		0xff		/* 自己検出(通信の全てと統合)	*/

/*検出方法*/		//20201002_異常履歴範囲拡大_43MA_chata
#define	ERR_DETECTION_RECEIVE		0	/* 通報							*/
#define	ERR_DETECTION_SELF			1	/* 自己検出						*/

//アプリ通信対応_異常情報管理_202409_SEC kashiwagi↓
/* 異常リスト */
// #define	ERRINFO_LIST_MAX	16						//!< 異常リスト配列最大  // MA_ERRINFO_LIST_MAX
#define	ERRINFO_LIST_MAX	64						//!< 異常リスト配列最大  // MA_ERRINFO_LIST_MAX
#define	ERRINFO_LATEST		ERRINFO_LIST_MAX		//!< 異常リスト最新取得
//アプリ通信対応_異常情報管理_202409_SEC kashiwagi↑

// 6201異常対策 msw_nagasi_20201005_start
/****************************************************************************
 *	エラー応答受信検知回数
 ****************************************************************************/
#define	ERR_DTT_NUM_E2P			7	/* 不揮発メモリ					*/
#define	ERR_DTT_NUM_RTC			7	/* RTC							*/
#define	ERR_DTT_NUM_SERIAL		7	/* シリアルFLASH				*/
#define	ERR_DTT_NUM_FLASH		7	/* 内蔵Flash					*/
/****************************************************************************
 *	BSPからの完了結果	//国内版SF01CR_リトライ制御	_MSW_chata_20200807
 ****************************************************************************/
#define	OK_FROM_BSP		0			/* 正常							*/
#define	NG_FROM_BSP		1			/* 異常							*/
// 6201異常対策 msw_nagasi_20201005_end


/****************************************************************************
 *	室温センサー
 ****************************************************************************/

/****************************************************************************
 *	湿度センサー
 ****************************************************************************/

/****************************************************************************
 *	時計
 ****************************************************************************/
#define	SUNDAY			0			/* 日							*/
#define	MONDAY			1			/* 月							*/
#define	TUESDAY			2			/* 火							*/
#define	WEDNESDAY		3			/* 水							*/
#define	THURSDAY		4			/* 木							*/
#define	FRIDAY			5			/* 金							*/
#define	SATURDAY		6			/* 土							*/

#define	CLOCK_INI		0			/* 時計時刻未設定				*/	//[A_018 NS_KOYAMA 080614]
#define	CLOCK_SET		1			/* 時計時刻設定済み				*/	//[A_018 NS_KOYAMA 080614]
	
/****************************************************************************
 *	操作禁止
 ****************************************************************************/
#define	PERMIT			0			/* 許可							*/
#define	PROHIBIT		1			/* 禁止							*/

/****************************************************************************
 *	オンオフタイマー
 ****************************************************************************/
#define	ONOFF_REPEAT		0			/* 繰り返し						*/
#define	ONOFF_ONCE		1			/* 単発							*/

#define	ONOFF_YET		0		// オンオフ未実行				// [A_026 NS_KOYAMA 080728]
#define	ONOFF_DONE		1		// オンオフ実行済み				// [A_026 NS_KOYAMA 080728]


/****************************************************************************
 *	消し忘れ防止タイマー
 ****************************************************************************/

/****************************************************************************
 *	週間スケジュールタイマー
 ****************************************************************************/
#define	WEEKLYTIMER_NONE	0			/* 設定なし						*/
#define	WEEKLYTIMER_START	1			/* 運転開始						*/
#define	WEEKLYTIMER_STOP	2			/* 停止							*/
#define	WEEKLYTIMER_AUTO2	3			/* 自動二値						*/		// 35MA 131203 chata B3コマンド対応・自動二値
#define	WEEKLYTIMER_FAN		4			/* 送風							*/		//2023/06/26 SEC A.Shinbori[45MA-SE/E03]
#define WEEKLYTIMER_VENT_LOW	5		/* 換気弱						*/		//47MA_換気スケジュールアプリ通信対応_TS_maruyama_20241023
#define WEEKLYTIMER_VENT_HIGH	6		/* 換気強						*/		//47MA_換気スケジュールアプリ通信対応_TS_maruyama_20241023
#define WEEKLYTIMER_VENT_STOP	7		/* 換気停止						*/		//47MA_換気スケジュールアプリ通信対応_TS_maruyama_20241023
	
#define	TEMP_NONE			63			/* 温度未設定					*/

#define WEEKLY_SCHEDULE_SELECT_NONE	0	/* 週間スケジュール動作設定 無効	*/	// 36MA 20140826 ogawa 週間スケジュール2枚化(追加)
#define WEEKLY_SCHEDULE_SELECT_1	1	/* 週間スケジュール動作設定 設定1	*/	// 36MA 20140826 ogawa 週間スケジュール2枚化(追加)
#define WEEKLY_SCHEDULE_SELECT_2	2	/* 週間スケジュール動作設定 設定2	*/	// 36MA 20140826 ogawa 週間スケジュール2枚化(追加)

#define SCHEDULE_NUM_1		0			/* 週間スケジュール設定 設定1 */		// 36MA 20140826 ogawa 週間スケジュール2枚化(追加)
#define SCHEDULE_NUM_2		1			/* 週間スケジュール設定 設定2 */		// 36MA 20140826 ogawa 週間スケジュール2枚化(追加)
#define SCHEDULE_NUM_MAX	2			/* 週間スケジュール設定 設定最大数 */	// 36MA 20140826 ogawa 週間スケジュール2枚化(追加)


/****************************************************************************
 *	設定温度範囲制限
 ****************************************************************************/

/****************************************************************************
 *	操作ロック
 ****************************************************************************/
#define	OPRUNLOCK			0			/* 解除							*/
#define	OPRLOCK				1			/* ロック						*/

/****************************************************************************
 *	設定温度自動復帰
 ****************************************************************************/
#define	AUTORETURN_TEMP_MAX			30	//[A_026 NS_KOYAMA 080726]
#define	AUTORETURN_TEMP_MIN			17	//[A_026 NS_KOYAMA 080726]

#define	AUTORETURN_MIN_MAX			120	//[A_034 NS_KOYAMA 080822]
#define	AUTORETURN_MIN_MIN			30	//[A_034 NS_KOYAMA 080822]

/****************************************************************************
 *	画面表示
 ****************************************************************************/
#define	DISPLAY_SIMPLE		0			/* 	操作画面簡易				*/
#define	DISPLAY_NORMAL		1			/* 	操作画面詳細				*/
#define DISPLAY_MIX			2			/*	日英併記					*/
#define DISPLAY_AIRFLOW     3			/*	空調換気					*/

/****************************************************************************
 *	輝度
 ****************************************************************************/
#define BRIGHTNESS_LO		0			// 弱
#define BRIGHTNESS_MID		1			// 中
#define BRIGHTNESS_HI		2			// 強

/****************************************************************************
 *	リモコン表示設定
 ****************************************************************************/
#define	CLOCKSTYLE_AMPM12H	0			/* AMHH:MM						*/
#define	CLOCKSTYLE_24H		1			/* ２４時間						*/
#define	CLOCKSTYLE_12HAMPM	2			/* HH:MMAM						*/

#define	TEMP_C			0			/* 温度１ﾟC表示					*/
#define	TEMP_F			1			/* 温度１ﾟF表示					*/
#define	TEMP_05C		2			/* 温度0.5ﾟC表示				*/	//[A_018 NS_KOYAMA 080620]

//[37MA A01 chata 20151222]
// バックライト点灯時間
#define BACKLIGHT_05			0		// 5秒
#define BACKLIGHT_10			1		// 10秒
#define BACKLIGHT_20			2		// 20秒
#define BACKLIGHT_30			3		// 30秒
#define BACKLIGHT_60			4		// 60秒

#ifdef COMPACT_MA
// サイン画面
#define SIGN_SET			0			// 設定温度
#define SIGN_ROOM			1			// 室内温度
#define SIGN_NONE			2			// 非表示
#endif	// COMPACT_MA

/****************************************************************************
 *	メンテナンス
 ****************************************************************************/
/****************************************************************************
 *	BLEメンテナンス
 ****************************************************************************/
/* BLEメンテナンス収集結果 */
enum{
	BLE_MENTE_COLLECT_RESULT_NON = 0				,		/* 未完了                    */
	BLE_MENTE_COLLECT_RESULT_CANCEL					,		/* 中断                      */
	BLE_MENTE_COLLECT_RESULT_EXCEPT_BLE_DISCONNECT	,		/* 異常終了(BLE通信継続不可) */
	BLE_MENTE_COLLECT_RESULT_EXCEPT_BLE_CONNECT		,		/* 異常終了(BLE通信継続可)   */
	BLE_MENTE_COLLECT_RESULT_NOT					,		/* 未対応                    */
	BLE_MENTE_COLLECT_RESULT_COMP					,		/* 正常終了                  */
	BLE_MENTE_COLLECT_RESULT_MAX
};

/* BLEメンテナンスシーケンス種別 */
enum{
	BLE_MENTE_SEQ_TYPE_NON = 0											,		/* シーケンスなし                                       */
	BLE_MENTE_SEQ_TYPE_OPEDATA_COLLECT_NON_ANALYSIS						,		/* 運転データ収集(診断無し p-h線図)シーケンス           */
	BLE_MENTE_SEQ_TYPE_OPEDATA_CUR_COLLECT_ANALYSIS						,		/* 運転データ収集(診断有り p-h線図)【現在値】シーケンス */
	BLE_MENTE_SEQ_TYPE_OPEDATA_PREV_COLLECT_ANALYSIS					,		/* 運転データ収集(診断有り p-h線図)【過去値】シーケンス */
	BLE_MENTE_SEQ_TYPE_FNCMON											,		/* 機能選択ありモニタ        */								//未使用(機能としては存在するが、シーケンス種別に反映はしていない)
	BLE_MENTE_SEQ_TYPE_FNCSET											,		/* 機能選択あり設定        */								//未使用(機能としては存在するが、シーケンス種別に反映はしていない)
	BLE_MENTE_SEQ_TYPE_OPEDATA_COLLECT_NON_ANALYSIS_MENT2_SESSION		,		/* MENTE2セッションによる運転データ収集(診断無し p-h線図)シーケンス           */		
	BLE_MENTE_SEQ_TYPE_REQUEST_CODE										,		/* 要求コード送信シーケンス                             */	//45MA-SE_BLE_要求コード送信_MESW_Masuda_20230809
	BLE_MENTE_SEQ_TYPE_FPFNCSEL_MON										,		/* マルチ機能選択モニタシーケンス                       */	//45MA-SE_BLE_マルチ機能選択_MESW_inoue_20230830
	BLE_MENTE_SEQ_TYPE_FPFNCSEL_SET										,		/* マルチ機能選択設定シーケンス                         */	//45MA-SE_BLE_マルチ機能選択_MESW_inoue_20230830
#ifdef USE_SE
	BLE_MENTE_SEQ_TYPE_DMNDANLGINPT_MON									,		/* デマンドアナログ入力設定モニタシーケンス             */	//45MA-SE_BLE_デマンドアナログ入力設定_MESW_inoue_20230830
	BLE_MENTE_SEQ_TYPE_DMNDANLGINPT_SET									,		/* デマンドアナログ入力設定設定シーケンス               */	//45MA-SE_BLE_デマンドアナログ入力設定_MESW_inoue_20230830
#endif
	BLE_MENTE_SEQ_TYPE_REQUEST_CODE_HOLD								,		/* 要求コード送信(ホールド)シーケンス					*/	//46MA BLE_要求コード送信(ホールド)_MESW_Masuda_20231208
	BLE_MENTE_SEQ_TYPE_MAX
};

/* BLEメンテナンス対象時点 */
enum{
	BLE_MENTE_POINT_NON = 0						,		/* 時点なし            */
	BLE_MENTE_POINT_CURRENT						,		/* 現在                */
	BLE_MENTE_POINT_EXCEPTION					,		/* 異常発生時          */
	BLE_MENTE_POINT_EXCEPTION_BEFORE_THREE_MIN	,		/* 異常停止3分前データ */
	BLE_MENTE_POINT_EXCEPTION_BEFORE_SIX_MIN	,		/* 異常停止6分前データ */
	BLE_MENTE_POINT_EXCEPTION_BEFORE_NINE_MIN	,		/* 異常停止9分前データ */
	BLE_MENTE_POINT_MAX
};

/* BLEメンテナンス中断種別 */
enum{
	BLE_MENTE_CANCEL_TYPE_CANCEL = 0				,		/* 中断                  */
	BLE_MENTE_CANCEL_TYPE_EXCEPTION_BLE_DISCONNECT	,		/* 異常(BLE通信継続不可) */
	BLE_MENTE_CANCEL_TYPE_EXCEPTION_BLE_CONNECT		,		/* 異常(BLE通信継続可)   */
	BLE_MENTE_CANCEL_TYPE_MAX
};

/* BLEメンテナンスセッション種別 */
enum{
	BLE_MENTE_SESSION_TYPE_NON_FUNC = 0		,		/* 機能選択中なし */
	BLE_MENTE_SESSION_TYPE_FUNC				,		/* 機能選択中あり */
	BLE_MENTE_SESSION_TYPE_FUNCSEL			,		/* 機能選択 	  */
	BLE_MENTE_SESSION_TYPE_MAX
};

/* BLEメンテナンス実施種別 */
enum{
	BLE_MENTE_EXE_TYPE_START = 0		,		/* 開始     */
	BLE_MENTE_EXE_TYPE_CANCEL			,		/* 中断     */
	BLE_MENTE_EXE_TYPE_END_NORMAL		,		/* 正常終了 */
	BLE_MENTE_EXE_TYPE_END_EXCEPTION	,		/* 異常終了 */
	BLE_MENTE_EXE_TYPE_MAX
};

/* BLEメンテナンス機能選択情報 */
enum{
	BLE_MENTE_FUNC_TYPE_NON = 0		,		/* 機能選択中なし */
	BLE_MENTE_FUNC_TYPE_EXIST		,		/* 機能選択中あり */
	BLE_MENTE_FUNC_TYPE_STOP		,		/* シーケンス停止 */						//BLEシーケンス中断不具合修正_MESW_inoue_20230915
	BLE_MENTE_FUNC_TYPE_MAX
};

/* BLEメンテナンス終了タイマ情報 */
enum{
	APL_BLE_EXIT_TIMER_STOP=0,		/* 停止中 */
	APL_BLE_EXIT_TIMER_RUN			/* 起動中 */
};

struct drvinfo_format{
	UH mode;
	UB datamode;
};

struct aplp_drvinfo_getdt {
//	struct drvinfo_format result[24];
	struct drvinfo_format result[26];		//DRVINFO_RESULT_MAXと紐づけること			//msw_chata_20230130_45MA_運転データ収集改善対応_仕様変更GUIとBLEからの運転データ収集シーケンスを共通化(DBも共通化)
};

/* BLE  機能選択 中断有無 */
enum{
    BLE_FNCSEL_COLLECT_RESULT_CONNECT = 0,		/* 接続中                    */
    BLE_FNCSEL_COLLECT_RESULT_CANCEL,			/* 中断                      */      
    BLE_FNCSEL_COLLECT_RESULT_MAX
};

/* BLEメンテナンス(機能選択)終了タイマ状態 */
enum{
	BLE_MENTE_EXIT_STS_TIMER_STOP=0,
	BLE_MENTE_EXIT_STS_TIMER_RUN
};

/****************************************************************************
 *	空調機型名情報
 ****************************************************************************/
#define	UNITINFO_SINGLE		0			/* 冷媒タイプ：シングル		*/
#define	UNITINFO_TWIN		1			/* 冷媒タイプ：ツイン		*/
#define	UNITINFO_TRIPLE		2			/* 冷媒タイプ：トリプル		*/
#define	UNITINFO_FOURTH		3			/* 冷媒タイプ：フォース		*/

/****************************************************************************
 *	室外機型名情報
 ****************************************************************************/

/****************************************************************************
 *	室内機型名情報
 ****************************************************************************/

/****************************************************************************
 *	室内外機型名情報	[A_025 honda 080725]
 ****************************************************************************/
/* storage_flgパラメータ */
#define UNITINFO_REGIST		0x00		/* [A_025 honda 080725]型名、製造No登録 */
#define UNITINFO_DELET		0xff		/* [A_025 honda 080725]型名、製造No消去 */

/* resultパラメータ */
#define	UNITINFO_RESULT_OK	0x00		/* [A_025 honda 080725]型名、製造No登録結果　OK */
#define	UNITINFO_RESULT_NG	0xff		/* [A_025 honda 080725]型名、製造No登録結果　NG */

/****************************************************************************
 *	室内機A/FP識別型名情報
 ****************************************************************************/
#define	SLIMFPCONBI_SLIM	0			/* スリム						*/
#define	SLIMFPCONBI_FP		1			/* フリープラン					*/
#define	SLIMFPCONBI_FU		2			/* 外気処理						*/
	
/****************************************************************************
 *	室内機新旧識別型名情報
 ****************************************************************************/
#define	MODELCONBI_OLD		0			/* 旧機種のみ					*/
#define	MODELCONBI_NEW		1			/* 新機種のみ					*/
#define	MODELCONBI_MIXOLD	2			/* 新旧混在＆親機旧機種			*/
#define	MODELCONBI_MIXNEW	3			/* 新旧混在＆親機新機種			*/

/****************************************************************************
 *	リモコンジャンパー型名情報
 ****************************************************************************/

/****************************************************************************
 *	室内機モード制限
 ****************************************************************************/

/****************************************************************************
 *	室内機機能対応
 ****************************************************************************/
#define	COOLER				0			/* 冷専							*/
#define	HEATPUMP			1			/* ヒーポン						*/
#define	BURNHEATER			2			/* 燃焼暖房						*/
#define	HEATER				3			/* 暖専							*/	
	

#define	THRM_IC				0			/* 室内機サーミスタ				*/
#define	THRM_RC				1			/* リモコンサーミスタ			*/

#define	FAN_NONE			0			/* 風速なし						*/
#define	FAN_2STEP			1			/* 風速２段						*/
#define	FAN_3STEP			3			/* 風速３段						*/
#define	FAN_4STEP			2			/* 風速４段						*/

#define	TEMPRANGE_FP_BIT		0x04													//[A_016 NS_KOYAMA 080602]
#define	TEMPRANGE_SLIM_STD		0						/* スリム標準				*/	//[A_016 NS_KOYAMA 080602]
#define	TEMPRANGE_SLIM_MID		1						/* スリム中温				*/	//[A_016 NS_KOYAMA 080602]
#define	TEMPRANGE_SLIM_LOW		2						/* スリム低温				*/	//[A_016 NS_KOYAMA 080602]
#define	TEMPRANGE_SLIM_16_31	3						/* スリム１６℃～３１℃		*/	//[A_016 NS_KOYAMA 080602]
#define	TEMPRANGE_FP_STD		(0+TEMPRANGE_FP_BIT)	/* マルチ(FP)標準			*/	//[A_016 NS_KOYAMA 080602]
#define	TEMPRANGE_FP_MID		(1+TEMPRANGE_FP_BIT)	/* マルチ(FP)中温			*/	//[A_016 NS_KOYAMA 080602]
#define	TEMPRANGE_FP_LOW		(2+TEMPRANGE_FP_BIT)	/* マルチ(FP)低温			*/	//[A_016 NS_KOYAMA 080602]
#define	TEMPRANGE_FP_16_31		(3+TEMPRANGE_FP_BIT)	/* マルチ(FP)１６℃～３１℃	*/	//[A_016 NS_KOYAMA 080602]

#define	TEMPRANGE_ME_FP_BIT		0x04
#define	TEMPRANGE_ME_AIC_STD		0			/* AIC標準				*/
#define	TEMPRANGE_ME_AIC_MID		1			/* AIC中温				*/
#define	TEMPRANGE_ME_AIC_LOW		2			/* AIC低温				*/
#define	TEMPRANGE_ME_RACHAC		3			/* RACHAC				*/
#define	TEMPRANGE_ME_FP_STD		(0+TEMPRANGE_FP_BIT)	/* マルチ(FP)標準			*/
#define	TEMPRANGE_ME_FP_MID		(1+TEMPRANGE_FP_BIT)	/* マルチ(FP)中温			*/
#define	TEMPRANGE_ME_FP_LOW		(2+TEMPRANGE_FP_BIT)	/* マルチ(FP)低温			*/


#define	EXPMODE_NONE		0			/* 拡張機能対応なし				*/		//＜[A_018 NSCS_KABASAWA 080613]>
#define	EXPMODE_EXP			1			/* 拡張機能対応あり				*/		//＜[A_018 NSCS_KABASAWA 080613]>

#define IC_TYPE_UNIT_FP 		0 			/* フリープラン       */
#define IC_TYPE_UNIT_AIC 		1 			/* 直膨A制御IC*/
#define IC_TYPE_UNIT_RACHAC 		2 			/* RAC,HAC*/

#define	LR_FUKOU_STEP_NONE			0		/* 左右風向なし						*/	// [40MA国内1st MSW Miyazaki]
#define	LR_FUKOU_STEP_3STEP			1		/* 左右風向３段						*/	// [40MA国内1st MSW Miyazaki]
#define	LR_FUKOU_STEP_7STEP			2		/* 左右風向７段						*/	// [40MA国内1st MSW Miyazaki]
#define	LR_FUKOU_STEP_UNDEFINED		3		/* 左右風向未定義					*/	// [40MA国内1st MSW Miyazaki]
/****************************************************************************
 *	サービス情報
 ****************************************************************************/
#define INIT_SALES_SHOP_NAME		""
#define INIT_SALES_SHOP_TEL			""
#define INIT_SERVICE_SHOP_NAME		{0xAF,0xB1,0x89,0x2D,0xAC,0x8B,0x8C,0x92,0xA0}	//ワンコールシステム
#define INIT_SERVICE_SHOP_TEL		"0120-9-24365"

/****************************************************************************
 *	異常履歴
 ****************************************************************************/

/****************************************************************************
 *	自動診断
 ****************************************************************************/

/****************************************************************************
 *	冷媒量診断
 ****************************************************************************/

/****************************************************************************
 *	管理者パスワード設定
 ****************************************************************************/

/****************************************************************************
 *	メンテナンスパスワード設定
 ****************************************************************************/

#define	TEMP_STD_1		17	// 標準	[A_017 TNN_Tanaka 080610]
#define	TEMP_STD_2		19	// 標準	[A_017 TNN_Tanaka 080610]
#define	TEMP_STD_3		28	// 標準	[A_017 TNN_Tanaka 080610]
#define	TEMP_STD_4		30	// 標準	[A_017 TNN_Tanaka 080610]
#define	TEMP_STD_5		12	// 標準 (ナイトセットバック対応) [30MAAU_A_02 SEC Nanjo.Y.]
#define	TEMP_MID_1		14	// 中温	[A_017 TNN_Tanaka 080610]
#define	TEMP_MID_2		17	// 中温	[A_017 TNN_Tanaka 080610]
#define	TEMP_MID_3		28	// 中温	[A_017 TNN_Tanaka 080610]
#define	TEMP_MID_4		30	// 中温	[A_017 TNN_Tanaka 080610]
#ifdef USE_SE
#define	TEMP_MID_5		12	// 中温							//45MA-SE_BLE_設定温度範囲_MESW_Masuda_20230904
#define	TEMP_MID_6		17	// 中温							//45MA-SE_BLE_設定温度範囲_MESW_Masuda_20230904
#endif
#define	TEMP_LOW_1		5	// 低温	[A_017 TNN_Tanaka 080610]
#define	TEMP_LOW_2		10	// 低温	[A_017 TNN_Tanaka 080610]
#define	TEMP_LOW_3		28	// 低温	[A_017 TNN_Tanaka 080610]
#define	TEMP_LOW_4		30	// 低温	[A_017 TNN_Tanaka 080610]
#define	TEMP_LOW_5		8	// 低温	[A_027 TNN_Tanaka 080731]
#ifdef USE_SE
#define	TEMP_LOW_6		7	// 低温							//45MA-SE_BLE_設定温度範囲_MESW_Masuda_20230904
#endif
#define	TEMP_1631_MIN	16	// 16～31℃機種 最小温度	[A_017 TNN_Tanaka 080610]
#define	TEMP_1631_MAX	31	// 16～31℃機種 最大温度	[A_017 TNN_Tanaka 080610]

#define	TEMP_COOL_MAX			30	//冷房最大値（温度）			// ishika120823 B3コマンド対応・設定温度範囲拡張
#define	TEMP_COOL_MIN			19	//冷房最小値（温度）			// ishika120823 B3コマンド対応・設定温度範囲拡張
#define	TEMP_HEAT_MAX			28	//暖房最大値（温度）			// ishika120823 B3コマンド対応・設定温度範囲拡張
#define	TEMP_HEAT_MIN			17	//暖房最小値（温度）			// ishika120823 B3コマンド対応・設定温度範囲拡張
#define	TEMP_NIGHT_MIN			12	//冷房最大値（温度）			// ishika120823 B3コマンド対応・設定温度範囲拡張

//設定温度自動復帰番号
#define AUTORETURN_ONE				0x01										//設定温度自動復帰タイマー					
#define AUTORETURN_TWO				0x02										//設定温度自動復帰(自動二値暖房用)タイマー		
#define AUTORETURN_BOTH				0x03										//設定温度自動復帰(両方)						
	
#define APL_TEMP_RANGE_DEFAULT		0	// 温度範囲設定値：デフォルト		[A_017 TNN_Tanaka 080616]			// <20120918_watanabe_自動二値範囲外時の制限メッセージ不具合>
#define APL_TEMP_RANGE_USERVALUE	1	// 温度範囲設定値：ユーザー設定値	[A_017 TNN_Tanaka 080616]			// <20120918_watanabe_自動二値範囲外時の制限メッセージ不具合>


//冷暖房連動処理結果用															//<20120918_watanabe_自動二値範囲外時の制限メッセージ不具合>
#define APL_AUTO_ADJUST_OK			0											//正常終了（冷房→暖房温度算出、暖房→冷房温度算出成功）
#define APL_AUTO_ADJUST_NG			1											//異常終了：温度範囲に入らず （冷房→暖房→冷房（もしくは逆）の算出結果が温度範囲に入らない）
#define APL_AUTO_ADJUST_OK_LIMIT	2											//正常終了：温度範囲に入ったが、連動処理により制限温度範囲で丸められた										// [+:kaishima20121005]
#define APL_AUTO_ADJUST_NG_LIMIT	3											//異常終了：制限温度範囲に入らず （冷房→暖房→冷房（もしくは逆）の算出結果が制限温度範囲に入らない）		// [+:kaishima20121005]
#define MODE_TYPE_COOL			0	// 運転モード 冷房
#define MODE_TYPE_HEAT			1	// 運転モード 暖房
#define MODE_TYPE_AUTO			2	// 運転モード 自動
#define MODE_TYPE_AUTO_2		3	// 運転モード 自動(二値)
	
#define CHLINK_COOL	0	//冷房制御時		//<20120802_watanabe_設定温度二値設定>
#define CHLINK_HEAT	1	//暖房制御時		//<20120802_watanabe_設定温度二値設定>
#define CHLINK_NONE	0xff	//補正不要		//<20120802_watanabe_設定温度二値設定>

/****************************************************************************
 *	立上げ
 ****************************************************************************/
#define START_UP_MIDST		0	//立上げ中
#define START_UP_OUTSIDE	1	//立上げ以外

 
/****************************************************************************
 *	デモ機能	[34MA A05 SEC hara 20121026]
 ****************************************************************************/
#define		DEMO_MODE_NONE	DB_DISABLE			// 通常
#define		DEMO_MODE_SET	DB_ENABLE			// デモモード

/****************************************************************************
 *	異常コード定義
 ****************************************************************************/
#define		EC_x1			0x01
#define		EC_x2			0x02
#define		EC_x3			0x03
#define		EC_x4			0x04
#define		EC_x5			0x05
#define		EC_x6			0x06
#define		EC_x7			0x07
#define		EC_x8			0x08
#define		EC_x9			0x09
#define		EC_xA			0x0a
#define		EC_xB			0x0b
#define		EC_xC			0x0c
#define		EC_xD			0x0d
#define		EC_xE			0x0e
#define		EC_xF			0x0f
#define		EC_x0			0x10
#define		EC_xH			0x11
#define		EC_xJ			0x12
#define		EC_xL			0x13
#define		EC_xP			0x14
#define		EC_xU			0x15

#define		EC_Ax			0x00
#define		EC_bx			0x20
#define		EC_Ex			0x40
#define		EC_Fx			0x60
#define		EC_Jx			0x80
#define		EC_Lx			0xa0
#define		EC_Px			0xc0
#define		EC_Ux			0xe0
/* 自己検出4桁異常コード */
#define		EC4_RTC			0x6202				//[A_02_hase_100713]
#define		EC4_EEPROM		0x6201				//[A_02_hase_100713]

/* 自己検出2桁異常コード */
#define		EC2_RTC			(EC_Ex | EC_x2)		//[A_02_hase_100713]
#define		EC2_EEPROM		(EC_Ex | EC_x1)		//[A_02_hase_100713]

/* BLE異常2桁コード */
#define		EC2_BLE			(EC_Ex | EC_x2)

// Kawa231113 start
// 冷媒漏洩4桁異常コード
#define EC4_LEAK_SELF		0x1521			// 接続ユニットにてR32システム冷媒漏洩異常
#define EC4_LEAK_OTHER		0x1522			// 他ユニットにてR32システム冷媒漏洩異常
#define EC4_LEAK_SAKIT		0x1524			// センサーアラームキットにてR32システム冷媒漏洩異常
#define EC4_LEAK_S_SELF		0x5558			// 接続ユニットの冷媒漏洩センサ不良・故障
#define EC4_C_LEAK_SELF		0x0910			// 【回路検査】接続ユニットにてR32システム冷媒漏洩検知
#define EC4_C_LEAK_OTHER	0x0911			// 【回路検査】他ユニットにてR32システム冷媒漏洩検知
#define EC4_C_LEAK_SAKIT	0x0912			// 【回路検査】センサーアラームキットにてR32システム冷媒漏洩検知
#define EC4_S_LIFE_SELF		0x1625			// 接続ユニットにて冷媒センサー寿命検知
#define EC4_S_LIFE_SAKIT	0x1626			// センサーアラームキットにて冷媒センサー寿命検知

// 冷媒漏洩系異常種別
#define LEAKTYPE_NONE		0		// 冷媒漏洩系異常なし
#define LEAKTYPE_LEAK		1		// 冷媒漏洩異常
#define LEAKTYPE_CIRCUIT	2		// 回路検査異常
#define LEAKTYPE_SENSOR		3		// センサ交換異常
// Kawa231113 end

//アプリ通信対応_異常情報管理_202409_SEC kashiwagi↓
#define ERRINFO_TYPE_DEF				(0)		// 判定後異常種別・優先度 未判定(未格納)
#define ERRINFO_TYPE_MENTE_SELF			(1)		// 判定後異常種別・優先度 メンテ異常(自己検出)
#define ERRINFO_TYPE_MENTE				(2)		// 判定後異常種別・優先度 メンテ異常
#define ERRINFO_TYPE_CIRCUIT			(3)		// 判定後異常種別・優先度 回路検査異常
#define ERRINFO_TYPE_ERROR_SELF			(4)		// 判定後異常種別・優先度 通常異常(自己検出)
#define ERRINFO_TYPE_ERROR				(5)		// 判定後異常種別・優先度 通常異常
#define ERRINFO_TYPE_LEAK				(6)		// 判定後異常種別・優先度 冷媒漏洩異常
//アプリ通信対応_異常情報管理_202409_SEC kashiwagi↑

#define EEPROM_INITIAL_NONE	0x0
#define EEPROM_INITIAL_RUN	0x1
#define EEPROM_INITIAL_DONE	0xFF

/*	設定温度範囲	*/
#define	TEMPINI(t)			{( (t)&0x01 ) , ( (t)>>1 ) }											//[A_026 NSCS_KABASAWA 080729]

/*	スリム冷房		*/
#define	SL_COOL_STD_LOWER		(19 << 1)	// 標準下限
#define	SL_COOL_STD_UPPER		(30 << 1)	// 標準上限
#define	SL_COOL_MID_LOWER		(14 << 1)	// 中温下限
#define	SL_COOL_MID_UPPER		(30 << 1)	// 中温上限
#define	SL_COOL_LOW_LOWER		(10 << 1)	// 低温下限
#define	SL_COOL_LOW_UPPER		(30 << 1)	// 低温上限
#define	SL_COOL_16_31_LOWER		(16 << 1)	// RAC下限
#define	SL_COOL_16_31_UPPER		(31 << 1)	// RAC上限

/*	スリム自動		*/
#define	SL_AUTO_STD_LOWER		(19 << 1)	// 標準下限
#define	SL_AUTO_STD_UPPER		(28 << 1)	// 標準上限
#define	SL_AUTO_MID_LOWER		(14 << 1)	// 中温下限
#define	SL_AUTO_MID_UPPER		(28 << 1)	// 中温上限
#define	SL_AUTO_LOW_LOWER		(10 << 1)	// 低温下限
#define	SL_AUTO_LOW_UPPER		(28 << 1)	// 低温上限
#define	SL_AUTO_16_31_LOWER		(16 << 1)	// RAC下限
#define	SL_AUTO_16_31_UPPER		(31 << 1)	// RAC上限

/*	スリム暖房		*/
#define	SL_HEAT_STD_LOWER		(17 << 1)	// 標準下限
#define	SL_HEAT_STD_UPPER		(28 << 1)	// 標準上限
#define	SL_HEAT_MID_LOWER		(14 << 1)	// 中温下限
#define	SL_HEAT_MID_UPPER		(28 << 1)	// 中温上限
#define	SL_HEAT_LOW_LOWER		(10 << 1)	// 低温下限
#define	SL_HEAT_LOW_UPPER		(28 << 1)	// 低温上限
#define	SL_HEAT_16_31_LOWER		(16 << 1)	// RAC下限
#define	SL_HEAT_16_31_UPPER		(31 << 1)	// RAC上限


/*	フリープラン冷房		*/
#define	FP_COOL_STD_LOWER		(19 << 1)	// 標準下限
#define	FP_COOL_STD_UPPER		(30 << 1)	// 標準上限
#define	FP_COOL_MID_LOWER		(14 << 1)	// 中温下限
#define	FP_COOL_MID_UPPER		(30 << 1)	// 中温上限
#define	FP_COOL_LOW_LOWER		( 8 << 1)	// 低温下限								//[A_029 NSCS_KABASAWA 080731]
#define	FP_COOL_LOW_UPPER		(30 << 1)	// 低温上限
#define	FP_COOL_16_31_LOWER		(16 << 1)	// RAC下限
#define	FP_COOL_16_31_UPPER		(31 << 1)	// RAC上限

/*	フリープラン自動		*/
#define	FP_AUTO_STD_LOWER		(19 << 1)	// 標準下限
#define	FP_AUTO_STD_UPPER		(28 << 1)	// 標準上限
#define	FP_AUTO_MID_LOWER		(17 << 1)	// 中温下限
#define	FP_AUTO_MID_UPPER		(28 << 1)	// 中温上限
#define	FP_AUTO_LOW_LOWER		( 8 << 1)	// 低温下限								//[A_027 NSCS_KABASAWA 080730]
#define	FP_AUTO_LOW_UPPER		(28 << 1)	// 低温上限
#define	FP_AUTO_16_31_LOWER		(16 << 1)	// RAC下限
#define	FP_AUTO_16_31_UPPER		(31 << 1)	// RAC上限

/*	フリープラン暖房		*/
#define	FP_HEAT_STD_LOWER		(17 << 1)	// 標準下限
#define	FP_HEAT_STD_UPPER		(28 << 1)	// 標準上限
#define	FP_HEAT_MID_LOWER		(17 << 1)	// 中温下限
#define	FP_HEAT_MID_UPPER		(28 << 1)	// 中温上限
#define	FP_HEAT_LOW_LOWER		( 8 << 1)	// 低温下限								//[A_027 NSCS_KABASAWA 080730]
#define	FP_HEAT_LOW_UPPER		(28 << 1)	// 低温上限
#define	FP_HEAT_16_31_LOWER		(16 << 1)	// RAC下限
#define	FP_HEAT_16_31_UPPER		(31 << 1)	// RAC上限

/****************************************************************************
 *	初期化要求
 ****************************************************************************/
#define INITIAL_REQUEST_NONE	0	//なし
#define INITIAL_REQUEST_INITIAL	1	//初期化中
#define INITIAL_REQUEST_RESET	2	//リセット中

#if defined(ROM_MAA) || defined(ROM_MAAC)
/****************************************************************************		//SEC 2016.06.01 sugimoto 国内外統合
 *	ナイトセットバック運転中														//SEC 2016.06.01 sugimoto 国内外統合
 ****************************************************************************/		//SEC 2016.06.01 sugimoto 国内外統合
#define		SETBACK_OFF			0		//SEC 2016.06.01 sugimoto 国内外統合 ナイトセットバック停止中
#define		SETBACK_ON			1		//SEC 2016.06.01 sugimoto 国内外統合 ナイトセットバック運転中
#define		SETBACK_WAIT		2		//SEC 2016.06.01 sugimoto 国内外統合 ナイトセットバック待機中
#endif	// ROM_MAA or ROM_MAAC

/****************************************************************************		//SEC 2016.06.01 sugimoto 国内外統合 Ａ／Ｋ制御対応
 *	スリム機種 A/K 制御	[30MAAC_A06a SEC Nanjo.Y. 20111014]							//SEC 2016.06.01 sugimoto 国内外統合 Ａ／Ｋ制御対応
 ****************************************************************************/		//SEC 2016.06.01 sugimoto 国内外統合 Ａ／Ｋ制御対応
#define		AK_SETTING_A	0					// A制御							//SEC 2016.06.01 sugimoto 国内外統合 Ａ／Ｋ制御対応
#define		AK_SETTING_K	1					// K制御							//SEC 2016.06.01 sugimoto 国内外統合 Ａ／Ｋ制御対応

/****************************************************************************		//SEC 2016.06.01 sugimoto 国内外統合 Ａ／Ｋ制御対応
 *	スケジュール臨時実行要求・フラグset用定数							//SEC 2016.06.01 sugimoto 国内外統合 Ａ／Ｋ制御対応
 ****************************************************************************/		//SEC 2016.06.01 sugimoto 国内外統合 Ａ／Ｋ制御対応
#define SCHE_EXE_REQ_NONE		0x00000000l		// (要求なし)
#define SCHE_EXE_REQ_WEEKLY		0x00000001l		// 週間スケジュール
#define SCHE_EXE_REQ_ONOFF		0x00000002l		// オンオフタイマー
//#ifdef MODEL_MA
#define SCHE_EXE_REQ_OC			0x00000004l		// 室外機静音運転スケジュール
#define SCHE_EXE_REQ_ECO		0x00000008l		// 省エネスケジュール
#define SCHE_EXE_REQ_CLEAN		0x00000010l		// 自動清掃
#define SCHE_EXE_REQ_FU_WEEKLY	0x00000020l		// 外気用週間スケジュール 	[32MA A03 100924 sugimoto]単体不具合
#define SCHE_EXE_REQ_FU_ONOFF	0x00000040l		// 外気用オンオフタイマ		[32MA A03 100924 sugimoto]単体不具合
#define SCHE_EXE_REQ_FU_NGT		0x00000080l		// 外気用週間ナイトﾊﾟｰｼﾞ 	[32MA A03 100924 sugimoto]単体不具合
#define SCHE_EXE_SUMMERTIME		0x00000100l		// サマータイム
#define SCHE_EXE_CLOCK_SETTING	0x00000200l		// 時刻設定		// 国内版43MA_室内機への時刻設定対応_kameyama_20201120
#define SCHE_EXE_REQ_PINCODE	0x00000400l		// PINコード自動更新
//#endif
#if defined(ROM_MAA) || defined(ROM_MAAC)
#define SCHE_EXE_SETBACK		0x00000800l		// セットバック[30MAA honda 100621]
#endif	// ROM_MAA or ROM_MAAC
#define SCHE_EXE_ELECENERGY		0x00001000L		// 電力量収集	// Kawa181115 電力量収集機能追加
//#define SCHE_EXE_REQ_ALL		0x1F		// (全指定)
#define SCHE_EXE_REQ_ALL		0xffffffffl		// (全指定)					[32MA A03 100924 sugimoto]単体不具合

#ifdef ROM_MAAU
/****************************************************************************
* 	ホールド状態				//[A_02_honda_100702]
 ****************************************************************************/
#define		APL_HOLD_OFF		0									//ホールド無効
#define		APL_HOLD_ON			1									//ホールド有効
#endif	// ROM_MAAU

/****************************************************************************
 *	言語設定
 ****************************************************************************/
enum{
#ifdef ROM_MAAJ
	JAPANESE = 0,
#elif defined(ROM_MAA)
	ENGLISH = 0,
	FRANCAIS,
	ESPANOL,
	ITALY,
	PORTUGES,
	GREECE,
	TURKISH,
	SVENSKA,
	DEUTCH,
	DUTCH,
	RUSSIAN,
	CZECH,
	HUNGARIAN,
	POLISH,
#elif defined(ROM_MAAC)	// 32MAAC対応 SEC T.Fujimoto
	CHNSIMP = 0,
	CHNTRAD,
	ENGLISH,
#elif defined(ROM_MAAU)
	ENGLISH_US = 0,
	FRANCAIS,
	ESPANOL,
#endif
//[32MAA F00 2015/4/3 yasuda] add-end
	LANG_MAX	//最大言語数
};
/*****************************************************************
 *	Bluetooth設定
 *****************************************************************/ 
// BLE更新設定
#define BLECOM_DISABLE		0			/* 無効								*/
#define BLECOM_ENABLE		1			/* 有効								*/


/****************************************************************************
 *	ロゴ画像更新
 ****************************************************************************/
// BLE更新状態
#define BLEUPSTATUS_0		0			/* 0%								*/
#define BLEUPSTATUS_5		5			/* 5%								*/
#define BLEUPSTATUS_10		10			/* 10%								*/
#define BLEUPSTATUS_20		20			/* 20%								*/
#define BLEUPSTATUS_30		30			/* 30%								*/
#define BLEUPSTATUS_40		40			/* 40%								*/
#define BLEUPSTATUS_50		50			/* 50%								*/
#define BLEUPSTATUS_60		60			/* 60%								*/
#define BLEUPSTATUS_70		70			/* 70%								*/
#define BLEUPSTATUS_80		80			/* 80%								*/
#define BLEUPSTATUS_90		90			/* 90%								*/
#define BLEUPSTATUS_END		100			/* 完了								*/
#define BLEUPSTATUS_NONE	0xFF		/* 未実施							*/

// BLE更新設定
#define BLEUPDATE_STOP		0			/* 中止								*/
#define BLEUPDATE_START		1			/* 開始								*/

/****************************************************************************
 *	形名
 ****************************************************************************/
// データサイズ
#define DSTTYPE_SIZE		18			/* リモコン形名						*/

/****************************************************************************
 *	製造番号
 ****************************************************************************/
// データサイズ
#define SERIALNO_SIZE		13			/* 製造番号							*/

// データ内容
#define SERIALNO_TYPE		3			/* 機種判別							*/
#define SERIALNO_DETAIL		4			/* 詳細仕様							*/
#define SERIALNO_VER		5			/* Ver.								*/

// 機種判別(文字)
#define SERIALTYPECHAR_CT01	'C'			/* CT01								*/
#define SERIALTYPECHAR_40MA	'S'			/* 40MA								*/

// 詳細仕様bit割り付け
#define SERIALDETAILBIT_DST	0x03		/* 向先								*/
#define SERIALDETAILBIT_SP	0x04		/* Standard/Premium					*/
#define SERIALDETAILBIT_BLE	0x08		/* BLE有無							*/

// 向先
#define SERIALDST_J			0x00		/* 国内								*/
#define SERIALDST_A			0x01		/* 欧州								*/
#define SERIALDST_U			0x02		/* 北米								*/
#define SERIALDST_C			0x03		/* 中国								*/
#define SERIALDST_MAX		4			/* 機種数							*/

// Standard/Premium
#define SERIALSP_STANDARD	0x00		/* Standard							*/
#define SERIALSP_PREMIUM	0x01		/* Premium							*/

// BLE有無
#define SERIALBLE_OFF		0x00		/* 無効								*/
#define SERIALBLE_ON		0x01		/* 有効								*/

/****************************************************************************
 *	 BLEセンサー受信状態	//国内版44MA_定国内版44MA_セントラルペリフェラルの自動切換え機能対応_chata_20211207
 ****************************************************************************/
#define BLE_SENSOR_NOT_FOUND		0			// 未検出
#define BLE_SENSOR_FOUND			1			// 検出中

#define BLE_SENSOR_CO2PPM_NODATA	0xFFFF		// データなし		// 国内版44MA_無線センサ対応_MSW inoue_20220106

/****************************************************************************
 *	空調換気設定情報		//国内版45MA_空調換気画面対応_MESW_Masuda_20230120
 ****************************************************************************/
#define VENT_NOT_STOP_AT_POWER_OFF	0	/* 空調機停止時の換気停止時に換気停止しない	*/
#define VENT_STOP_AT_POWER_OFF		1	/* 空調機停止時の換気停止時に換気停止する	*/

/****************************************************************************
 *	HAデバイス接続			//国内版45MA_空清(HAデバイス接続)表示対応_MESW_inoue_20221214
 ****************************************************************************/
#define	HA_DEVICE_NO_CONNECT		0	/* 接続無し							*/
#define	HA_DEVICE_CONNECT			1	/* 接続有り							*/

/****************************************************************************
 *	HAデバイス表示			//国内版45MA_空清(HAデバイス接続)表示対応_MESW_inoue_20221214
 ****************************************************************************/
#define	BLE_HA_DEVICE_NO_DISP		0	/* 非表示							*/
#define	BLE_HA_DEVICE_DISP			1	/* 表示								*/

/****************************************************************************
 *	内部クリーン制御停止	//国内版45MA_内部クリーン制御対応_MESW_inoue_20221215
 ****************************************************************************/
#define	INNER_CLEAN_STOP_NRML	0	/* 通常								*/
#define	INNER_CLEAN_STOP_STOP	1	/* 停止								*/

/****************************************************************************
 *	急速運転
 ****************************************************************************/
// 運転状態
#define	RAPIDDRIVE_OFF		0			/* 急速運転停止						*/
#define	RAPIDDRIVE_ON		1			/* 急速運転							*/

#ifdef COMPACT_MA
/****************************************************************************
 *	デザイン設定
 ****************************************************************************/
// カラー選択
#define SELECT_BLACK		0			/* 黒								*/
#define SELECT_WHITE		1			/* 白								*/
#define SELECT_COLOR		2			/* カラー							*/
#define SELECT_EXTERNAL		3			/* 外部端末							*/

// カラー適用
#define APPLICATION_CHAR	0			/* 文字								*/
#define APPLICATION_BG		1			/* 背景								*/

/****************************************************************************
 *	シリアルフラッシュアドレス
 ****************************************************************************/
// 開始位置
#define START_SERIAL_ADDR	0x00010000L	/* シリアルフラッシュ開始アドレス	*/

/****************************************************************************
 *	ExRomIconリスト
 ****************************************************************************/
// リスト
#define ROMICON_LIST_NUM	1			/* リスト数							*/

// アイコンID
#define ROMICON_ID_LOGO		0			/* ロゴ画像							*/

// 幅
#define ROMICON_WIDE_LOGO	320			/* ロゴ画像幅						*/

// 高さ
#define ROMICON_HIGH_LOGO	160			/* ロゴ画像高さ						*/

// シリアルフラッシュアドレス
#define ROMICON_ADR_LOGO	0x00010000L	/* ロゴ画像アドレス(Block1セクタ0)	*/

// パレットID
#define ROMICON_PLT_LOGO	0x00		/* ロゴ画像パレットID				*/

// 高さ
#define ROMICON_DATA_SIZE	128			/* ロゴ画像圧縮データ1単位サイズ	*/

/****************************************************************************
 *	Exパレットリスト
 ****************************************************************************/
// リスト
#define PALLET_LIST_NUM		9			/* リスト数							*/

// パレットID
#define PALLET_ID_BW		0			/* モノクロ白背景					*/
#define PALLET_ID_WB		1			/* モノクロ黒背景					*/
#define PALLET_ID_BCL		2			/* 黒文字明るい背景					*/
#define PALLET_ID_BCM		3			/* 黒文字中間背景					*/
#define PALLET_ID_WCM		4			/* 白文字中間背景					*/
#define PALLET_ID_WCD		5			/* 白文字暗い背景					*/
#define PALLET_ID_CW		6			/* カラー文字白背景					*/
#define PALLET_ID_CB		7			/* カラー文字黒背景					*/
#define PALLET_ID_EX		8			/* 外部端末							*/

// 基準パレットID
#define PALLET_ROM_BW		0			/* モノクロ白背景					*/
#define PALLET_ROM_WB		1			/* モノクロ黒背景					*/
#define PALLET_ROM_BCL		2			/* 黒文字明るい背景					*/
#define PALLET_ROM_BCM		3			/* 黒文字中間背景					*/
#define PALLET_ROM_WCM		4			/* 白文字中間背景					*/
#define PALLET_ROM_WCD		5			/* 白文字暗い背景					*/
#define PALLET_ROM_CW		6			/* カラー文字白背景					*/
#define PALLET_ROM_CB		7			/* カラー文字黒背景					*/

// RAMパレットID
#define PALLET_RAM_BLE		0			/* BLE								*/

// シリアルフラッシュアドレス
#define PALLET_ADR_PLT		0x00030000L	/* パレットアドレス(Block3セクタ0)	*/

// パレット
#define PALLET_SIZE_8		256			/* 8bit色パレットサイズ				*/
#define PALLET_COLOR_SIZE	  2			/* 1色のデータサイズ(2byte)			*/

/****************************************************************************
 *	タッチパネル補正情報
 ****************************************************************************/
// 補正種別
#define CALIBTYPE_SHIPMENT	0			/* 出荷検査							*/
#define CALIBTYPE_NORMAL	1			/* 通常								*/

// データサイズ
#define COORDINATE_SIZE		2			/* 座標軸数							*/
#define CALIBPOINT_SIZE		9			/* 補正点数							*/

#endif	// COMPACT_MA

/********	 メンテナンス設定(M-NETアドレス単位設定用)				********/
#define MNET_ADDR_MIN	0
#define MNET_ADDR_MAX	127

#define COMMON_E2P_ADDR	( 0 )	/*!< 共通データベースの格納EEPROM領域	*/
#define EEPROM_1ST_ACCESS_SIZE (4000)	/*!< 共通データベースの1回目の格納EEPROMサイズ	*/
#define COMMON_E2P_ADDR2	( 4096 )	/*!< 共通データベースの格納EEPROM領域	*/

/* ↓86RC-CHN_Buzzer_TS */
/****************************************************************************
 *	ブザー音量
 ****************************************************************************/
// Kawa230111 start
// ブザー鳴動制御
#define BUZZER_CTRL_STOP			0			// 止める
#define BUZZER_CTRL_TOUCH_SHORT		1			// 静電タッチキー 単押し
#define BUZZER_CTRL_TOUCH_LONG		2			// 静電タッチキー 長押し
#define BUZZER_CTRL_KEEP_RINGING	3			// 鳴動させ続ける
#define BUZZER_CTRL_IR_NORMAL		4			// 赤外線 正常受信
#define BUZZER_CTRL_IR_UNMATCH		5			// 自動二値状態不一致
#define BUZZER_CTRL_IR_PROHIBIT		6			// 操作禁止機能有り
#define BUZZER_CTRL_ALARM			7			// アラーム
#define BUZZER_CTRL_NUM				8			// 鳴動制御数
// Kawa230111 end

// ブザー音量
#define BUZZER_LEVEL_OFF			0			// 消音
#define BUZZER_LEVEL_SMALL			1			// 小
#define BUZZER_LEVEL_BIG			2			// 大

// Kawa221219 start
// ブザー鳴動状態
#define BUZZER_OFF					0			// 停止中
#define BUZZER_ON					1			// 鳴動中
// Kawa221219 end
/* ↑86RC-CHN_Buzzer_TS */

//アプリ通信対応_202409_SEC kashiwagi↓
// リモコンアラーム設定
#define RC_ALARM_OFF				0			// リモコンアラームOFF
#define RC_ALARM_ON					1			// リモコンアラームON
//アプリ通信対応_202409_SEC kashiwagi↑

/****************************************************************************
 *	換気動作設定詳細	// 国内版46MA_換気急速表示対応_MESW_tsuji_20231006
 ****************************************************************************/
//機種選択
#define VENT_KISYU_LOSSNAI				0		/* ロスナイ			*/
#define VENT_KISYU_IF_CONNECT_LOSSNAI	1		/* IF接続ロスナイ	*/
#define VENT_KISYU_FAN_1				2		/* 風量1段機種		*/
#define VENT_KISYU_FAN_2				3		/* 風量2段機種		*/

//不在時設定
#define ABSENCE_SET_DISABLE		0		/* 無効		*/
#define ABSENCE_SET_STOP		1		/* 停止		*/
#define ABSENCE_SET_LOW			2		/* 弱		*/
#define ABSENCE_SET_HI			3		/* 強		*/

//デフォルトリセットタイプ					//46MA 換気機器選択対応_MESW_Masuda_20231020
#define VENT_DETAIL_ALL_RESET		0	/* 機種選択と換気動作詳細設定のリセット	*/
#define VENT_DETAIL_SETTING_RESET	1	/* 換気動作詳細設定のリセット	*/
#define VENT_DETAIL_NOT_RESET		2	/* 換気動作詳細設定を保持	*/

/****************************************************************************
 *	リモコン換気制御IF切替情報	//46MA 換気機器選択対応_MESW_Masuda_20231020
 ****************************************************************************/
#define RC_VENT_CTRL_IF_NONE	0		/* 換気制御IF無し			*/
#define RC_VENT_CTRL_IF_FAN_1	1		/* 換気制御IF有り1段		*/
#define RC_VENT_CTRL_IF_FAN_2	2		/* 換気制御IF有り2段		*/
#define RC_VENT_CTRL_IF_LOSSNAY	3		/* 換気制御IF有りロスナイ	*/
#define RC_VENT_CTRL_IF_UNDEF	0xFF	/* 初期値 判定未実施状態	*/

/****************************************************************************
 *	換気操作情報				//46MA 換気機器選択対応_MESW_Masuda_20231020
 ****************************************************************************/
#define RC_VENT_CTRL_OPE_NONE	0		/* 換気操作未対応			*/
#define RC_VENT_CTRL_OPE_A		1		/* 換気操作A対応			*/
#define RC_VENT_CTRL_OPE_B		2		/* 換気操作B対応			*/
#define RC_VENT_CTRL_OPE_UNDEF	0xFF	/* 初期値 判定未実施状態	*/

// Kawa240925 start
/****************************************************************************
 *	ドライ設定
 ****************************************************************************/
#define DRYSETTING_NOCHANGE		0	// 変化無し
#define DRYSETTING_NORMAL		1	// 標準
#define DRYSETTING_STRONG		2	// 強
// Kawa240925 end

//47MA SEC A.Shinbori 2024.10 仕様変更(G07改訂_アプリ) ↓
/****************************************************************************
 *	回路検査実施時刻
 ****************************************************************************/
#define CIRCUIT_CHKTIME_MAX		16		// 回路検査実施履歴の最大件数(16件)
#define SAFETYDEV_CHK_NON		0xff	// 安全装置動作状態＝未実施
#define SAFETYDEV_CHK_COMP		0		// 安全装置動作状態＝動作完了
#define SAFETYDEV_CHK_END		1		// 安全装置動作状態＝動作終了

//47MA SEC A.Shinbori 2024.10 不具合修正(#67425/#67563) ↓
// 回路検査状態
#define CIRCUIT_CHK_STATE_INIT		0		// 回路検査未実施/終了
#define CIRCUIT_CHK_STATE_WAITING	1		// 回路検査終了待ち
//47MA SEC A.Shinbori 2024.10 不具合修正(#67425/#67563) ↑
//47MA SEC A.Shinbori 2024.10 仕様変更(G07改訂_アプリ) ↑

/****************************************************************************
 *	霜取時換気風量抑制			//47MA 霜取時換気風量抑制_MESW_Masuda_20241108
 ****************************************************************************/
#define DEFROST_VENT_SPEED_REDUCTION_NOT_CTRL	0		/* 通常			*/
#define DEFROST_VENT_SPEED_REDUCTION_CTRL		1		/* 制御中		*/

/****************************************************************************
 *	霜取中状態					//47MA 霜取時換気風量抑制_MESW_Masuda_20241127
 ****************************************************************************/
#define DEFROST_STOP							0		/* 霜取停止中	*/
#define DEFROST_START							1		/* 霜取運転中	*/

/*
 * --------------------------------------------------------------------------
 *   Style Define Area
 * --------------------------------------------------------------------------
 */
	
/********	 時刻		********/
struct time {						/* 時刻xx時yy分					*/
	UB			min;				/* 分	0x00-0x59(BCD)				*/
	UB			hour;				/* 時	0x00-0x23(BCD)				*/
};

/********	 日にち		********/
struct date {
	UB			day;				/* 日	0x01-0x31(BCD)				*/
	UB			month;				/* 月	0x01-0x12(BCD)				*/
	UB			year;				/* 年	0x00-0x99(BCD)				*/
};

/********	 年月		********/
struct yearmonth{
	UB			month;				/* 月	0x01-0x12(BCD)				*/
	UB			year;				/* 年	0x00-0x99(BCD)				*/
};

struct c {
	UB			point5	:1;			/* 小数部(0:0.0℃、1:0.5℃)			*/
	UB			integer	:7;			/* 整数部(Binary)					*/
};
union temp {
	struct	c	c;					/* セ氏								*/
	UB			f;					/* 華氏								*/
};

/********	 時計時刻		********/
struct clock {						/* 時計						*/
	UB			sec;				/* 秒(0-59 Binary)					*/
	struct time	time;				/* 時刻xx時yy分						*/
	struct date	date;				/* xx年xx月xx日						*/
	UB			week;				/* 曜日								*/
};

/********	 オンオフタイマー			********/
struct onoff_timer {
	UB			enable		:1;		/* 有効／無効						*/
	UB			mode		:1;		/* 繰り返し							*/
	UB			on_end_flg	:1;		/* オンタイマー実行済みフラグ		*/
	UB			off_end_flg	:1;		/* オフタイマー実行済みフラグ		*/
	struct time	start;				/* オンタイマー時刻					*/
	struct time	stop;				/* オフタイマー時刻					*/
};

/********	 消し忘れ防止タイマー			********/
struct autooff_timer {
	UB			enable	:1;			/* 有効／無効						*/
	UB			min		:8;			/* 時間  30分～240分(Binary)		*/
};

/********	 スリム/マルチ/外気共通週間スケジュールタイマー			********/
struct common_aweek_schedule {
	struct time	time_mx;		/* スリム/マルチ/外気   時刻						*/
	UB			sf_power_mx:3;	/* スリム/マルチ		発停動作(設定無し/運転開始/停止/自動二値/(送風)/換気弱/換気強/換気停止) */						//47MA_換気スケジュールアプリ通信対応_TS_maruyama_20241023
	UB			fu_power_mx:2;	/* 外気                 無/運転/停止				*/
	union temp	temp_mx;		/* スリム/マルチ        設定温度					*/
	union temp	temp2_mx;		/* スリム/マルチ        設定温度2（自動二値 上限。自動二値時はtempが下限となる）	*/
	UB			fansp_mc;		/* 外気                 風速						*/
};

struct common_weekly_schedule {
	struct common_aweek_schedule schedule_mx[WEEKLYTIMER_MAX];
};

/********	 週間スケジュールタイマー			********/

struct aweek_schedule {
	struct time	time;			/* 時刻								*/
	union temp	temp;			/* 設定温度		*/

	UB			power	:3;		/* 発停動作(設定無し/運転開始/停止/自動二値/(送風)/換気弱/換気強/換気停止) */											//47MA_換気スケジュールアプリ通信対応_TS_maruyama_20241023
	union temp	temp2_mx;		/* 設定温度2（自動二値 上限。自動二値時はtempが下限となる）	*/
};

struct weekly_schedule {
	struct aweek_schedule	schedule[WEEKLYTIMER_MAX];
};
	
/********	 設定温度範囲制限			********/
struct temp_limit {
	UB				enable	:1;		/* 有効／無効						*/
	UB			mode_exp_set_mc	:1;		/* 設定温度拡張					*/	
	union temp		cool_upper;		/* 冷房上限			*/
	union temp		cool_lower;		/* 冷房下限				*/
	union temp		heat_upper;		/* 暖房上限			*/
	union temp		heat_lower;		/* 暖房下限					*/
	union temp		auto_upper;		/* 自動上限					*/
	union temp		auto_lower;		/* 自動下限					*/
    UB              higher_limit_enable_mc  :1; /* 上位からの設定温度範囲制限   */  //[42MA SEC M.Matsumura 190823]
    UB              cool_highlim_mc  :1;        /* 冷房上位制限 */                  //[42MA SEC M.Matsumura 190930]
    UB              heat_highlim_mc  :1;        /* 暖房上位制限 */                  //[42MA SEC M.Matsumura 190930]
    UB              auto_highlim_mc  :1;        /* 自動上位制限 */                  //[42MA SEC M.Matsumura 190930]
};

/********	 操作ロック			********/
struct opr_lock {
	UB	enable		:1;	/*!<操作ロック設定	*/
	UB	dst_mc		:1;	/*!<向け先設定		*/
	UB	power		:1;	/*!<発停操作		*/
	UB	mode		:1;	/*!<運転モード操作	*/
	UB	temp		:1;	/*!<設定温度操作	*/
	UB	vane		:1;	/*!<風向操作		*/
	UB	menu		:1;	/*!<メニュー操作	*/
	UB	wind_mc		:1;	/*!<風速操作		*/
	UB	louver_mc	:1;	/*!<ルーバー操作	*/
#ifdef ROM_MAAU
	UB	hold_mc		:1;	/*!<HOLD			*/
#endif	// ROM_MAAU
	UB	fu_humid_mb	:1;	/*!<加湿モード操作	*/	//外処理
	UB	fu_vent_mb	:1;	/*!<換気モード操作	*/	//外処理
};

/********	 設定温度自動復帰			********/
struct temp_auto_return {
	UB			enable	:1;			/* 有効／無効						*/
	union temp	cooltemp;			/* 冷房温度			*/
	UB			cooltime;			/* 冷房時間							*/
	union temp	heattemp;			/* 暖房温度				*/
	UB			heattime;			/* 暖房時間							*/
};

#ifdef USE_SE															// ↓ 45MA-SE対応_特殊表示対応_MESW_inoue_20230530
/********	特殊表示設定					********/
struct sp_disp {
	UB	room_temp_mb		:1;	/*!<特殊表示時の吸込温度表示*/
	UB	set_temp_mb			:1;	/*!<特殊表示時の設定温度表示*/
};
#endif																	// ↑ 45MA-SE対応_特殊表示対応_MESW_inoue_20230530

/********	 リモコン表示設定			********/
struct rc_disp {
	UB	clock_enable		:1;	/*!<時刻表示				*/
	UB	clock_style			:2;	/*!<AM/PM表示				*/
	UB	temp_cf				:2;	/*!<温度単位表示			*/
#ifdef COMPACT_MA
	UB	sign_mc				:2;	/*!<サイン画面				*/
#else	// SMART_MA or SMART_ME
#endif	// COMPACT_MA
	UB	temp_enable			:1;	/*!<吸込温度表示			*/
	UB	autocoolheat_enable	:1;	/*!<自動冷暖表示	 		*/
	UB	automode_enable		:1;	/*!<自動モード使用設定		*/
	UB	bltime				:3;	/*!<ディスプレイ点灯		*/
	UB	led_mc				:1;	/*!<運転LED点灯				*/
#ifdef COMPACT_MA
	UB	logo_mc				:1;	/*!<ロゴ画像表示			*/
#endif	// COMPACT_MA
#ifdef USE_SE
	struct sp_disp sp_disp_mx;	/*!<特殊表示設定			*/			// 45MA-SE対応_特殊表示対応_MESW_inoue_20230530
#endif
};

/********	 サービス情報					********/
struct service_info {
	struct {
		UB		name[SHOPNAME_LEN+1];	/* 販売店名						*/
		UB		phone[TELNAME_LEN+1];	/* 販売店TEL					*/
	} shop;
	struct {
		UB		name[SHOPNAME_LEN+1];	/* サービス店名					*/
		UB		phone[TELNAME_LEN+1];	/* サービスTEL					*/
	} service;
};

struct err_log {					/* 異常履歴						*/
	UB	storage_flg;					/* 格納フラグ					*/
	struct date	date;					/* xx年xx月xx日					*/
	struct time	time;					/* 時刻xx時yy分					*/
	UB			adr;					/* 発生元アドレス				*/
	UB			goki;					/* 発生元号機					*/
	UB			codeshort;				/* ２桁異常コード				*/
	UH			codelong;				/* ４桁異常コード				*/
	UB			err_mente_mc;			/* 異常種別(異常orメンテ)			*/	//20201002_異常履歴範囲拡大_43MA_chata		//異常通報(struct error)と同じデータのため、同じアサイン名とする。
	UB			detection_method_mc;	/* 異常検出方法(通報or自己検出)		*/	//20201002_異常履歴範囲拡大_43MA_chata
	UB			atr_mc;					/* 異常ﾕﾆｯﾄ属性(OC,IC,..,--(未定義)	*/	//20201002_異常履歴範囲拡大_43MA_chata		//異常通報(struct error)と同じデータのため、同じアサイン名とする。
};

// ↓国内版43MA_異常猶予履歴対応_inoue_20200924
struct gra_err_log {					/* 異常猶予履歴					*/
	UB	storage_flg;					/* 格納フラグ					*/
	struct date	date;					/* xx年xx月xx日					*/
	struct time	time;					/* 時刻xx時yy分					*/
	UB			adr;					/* 発生元アドレス				*/
	UB			goki;					/* 発生元号機					*/
	UB			codeshort;				/* ２桁異常コード				*/
	UH			codelong;				/* ４桁異常コード				*/
};
// ↑国内版43MA_異常猶予履歴対応_inoue_20200924

struct rc_jp {
	UB			jp1		:1;				/* ＪＰ１						*/
	UB			jp2		:1;				/* ＪＰ２						*/
	UB			jp3		:1;				/* ＪＰ３						*/
	UB			jp4		:1;				/* ＪＰ４						*/
	UB			jp5		:1;				/* ＪＰ５						*/
	UB			jp6		:1;				/* ＪＰ６						*/
	UB			jp7		:1;				/* ＪＰ７						*/
	UB			jp8		:1;				/* ＪＰ８						*/
};

struct icoc_info {												//[A_024 NS_KOYAMA 080720]
	UB	storage_flg;						/* 格納フラグ		*/
	UB	addr;								/* 冷媒アドレス		*/
	UB	oc_name_hutai;						/* 室外機型名の付帯情報 */
	UB	oc_name_shikibetu;					/* 室外機型名の識別情報 */
	UB	oc_name[UNITNAME_LEN+1];			/* 室外機型名		*/
	UB	oc_serial_hutai;					/* 室外機製造番号の付帯情報 */
	UB	oc_serial_shikibetu;				/* 室外機製造番号の識別情報 */
	UB	oc_serial[UNITSERIAL_LEN+1];		/* 室外機製造番号	*/
	UB	oc_namecode_hutai;					/* 室外機型名コードの付帯情報 */
	UB	oc_namecode_shikibetu;				/* 室外機型名コードの識別情報 */
	UB	oc_namecode[NAMECODE_LEN+1];		/* 室外機型名コード	*/
	UB	ic_name_hutai[4];					/* 室内機型名の付帯情報 */
	UB	ic_name_shikibetu[4];				/* 室内機型名の識別情報 */
	UB	ic_name[4][UNITNAME_LEN+1];			/* 室内機型名		*/
	UB	ic_serial_hutai[4];					/* 室内機製造番号の付帯情報 */
	UB	ic_serial_shikibetu[4];				/* 室内機製造番号の識別情報 */
	UB	ic_serial[4][UNITSERIAL_LEN+1];		/* 室内機製造番号	*/
	UB	ic_namecode_hutai[4];				/* 室内機型名コードの付帯情報 */
	UB	ic_namecode_shikibetu[4];			/* 室内機型名コードの識別情報 */
	UB	ic_namecode[4][NAMECODE_LEN+1];		/* 室内機型名コード	*/
};
/* 下記は型名、製造番号、型名コードを同時にwriteすると					*/
/* サイズオーバーとなるため、3つに分割する(GUI→APLの設定時のみ使用)	*/
/* 型名設定用構造体 */
struct icoc_info_name {
	UB	storage_flg;						/* 格納フラグ  */
	UB	addr;								/* 冷媒アドレス  */
	UB	oc_name_hutai;						/* 室外機型名の付帯情報 */
	UB	oc_name_shikibetu;					/* 室外機型名の識別情報 */
	UB	oc_name[UNITNAME_LEN+1];			/* 室外機型名  */
	UB	ic_name_hutai[4];					/* 室内機型名の付帯情報 */
	UB	ic_name_shikibetu[4];				/* 室内機型名の識別情報 */
	UB	ic_name[4][UNITNAME_LEN+1];			/* 室内機型名  */
};
/* 製造番号設定用構造体 */
struct icoc_info_serial {
	UB	storage_flg;						/* 格納フラグ  */
	UB	addr;								/* 冷媒アドレス  */
	UB	oc_serial_hutai;					/* 室外機製造番号の付帯情報 */
	UB	oc_serial_shikibetu;				/* 室外機製造番号の識別情報 */
	UB	oc_serial[UNITSERIAL_LEN+1];		/* 室外機製造番号 */
	UB	ic_serial_hutai[4];					/* 室内機製造番号の付帯情報 */
	UB	ic_serial_shikibetu[4];				/* 室内機製造番号の識別情報 */
	UB	ic_serial[4][UNITSERIAL_LEN+1];		/* 室内機製造番号 */
};	
/* 型名コード設定用構造体 */
struct icoc_info_namecode {
	UB	storage_flg;						/* 格納フラグ  */
	UB	addr;								/* 冷媒アドレス  */
	UB	oc_namecode_hutai;					/* 室外機型名コードの付帯情報 */
	UB	oc_namecode_shikibetu;				/* 室外機型名コードの識別情報 */
	UB	oc_namecode[NAMECODE_LEN+1];		/* 室外機型名コード	*/
	UB	ic_namecode_hutai[4];				/* 室内機型名コードの付帯情報 */
	UB	ic_namecode_shikibetu[4];			/* 室内機型名コードの識別情報 */
	UB	ic_namecode[4][NAMECODE_LEN+1];		/* 室内機型名コード	*/
};

struct error {						/* 異常／メンテ						*/
	UB			err_mente	:2;		/* 異常解除／異常／メンテ			*/
	UB			unitadr;			/* 異常アドレス						*/	//[A_020 NS_KOYAMA 080701]
	UB			unitgoki;			/* 異常号機							*/	//[A_020 NS_KOYAMA 080701]
	UB			atr;				/* 異常機器属性						*/	//[A_020 NS_KOYAMA 080701]
	UB			codeshort;			/* ２桁異常コード					*/
	UH			codelong;			/* ４桁異常コード					*/
	struct date	date;				/* xx年xx月xx日						*/	//[37MA A01 chata 20151222]
	struct time	time;				/* 時刻xx時yy分						*/	//[37MA A01 chata 20151222]
	UB			mnetadr_mc;			// 送信M-NETアドレス(マルチのメンテコマンド用)	// Kawa231013
	UB 			storage_mc;			/* 異常格納状態						*/	//アプリ通信対応_異常情報管理_202409_SEC kashiwagi
	UB			errinfo_type_mc;	/* 判定後の異常種別			 		*/	//アプリ通信対応_異常情報管理_202409_SEC kashiwagi
};
// ↓国内版43MA_異常猶予履歴対応_inoue_20200924
/********	 異常猶予				********/
struct grace_error {				/* 異常猶予							*/
	UB			err_mente	:2;		/* 異常解除／異常／メンテ			*/
	UB			unitadr;			/* 異常アドレス						*/
	UB			unitgoki;			/* 異常号機							*/
	UB			atr;				/* 異常機器属性						*/
	UB			codeshort;			/* ２桁異常コード					*/
	UH			codelong;			/* ４桁異常コード					*/
	struct date	date;				/* xx年xx月xx日						*/
	struct time	time;				/* 時刻xx時yy分						*/
};
// ↑国内版43MA_異常猶予履歴対応_inoue_20200924
// 6201異常対策 msw_nagasi_20201005_start
/********	 エラー応答受信検知カウンタ		********/
struct err_dtt_cnt {
	UB			write_cnt_e2p_mc;				/* 不揮発メモリ書込み			*/
	UB			read_common_db_err_dtt_e2p_mc;	/* 不揮発メモリ共通DB異常検知	*/
	UB			write_cnt_rtc_mc;				/* RTC書込み					*/
	UB			read_cnt_rtc_mc;				/* RTC読込み					*/
	UB			write_cnt_e2flash_mc;			/* 内臓EEFLASH書込み			*/		//43MA_HWリトライ対応 msw_chata_20201013
	UB			read__err_dtt_e2flash_mc;		/* 内臓EEFLASH読込み異常検知	*/		//43MA_HWリトライ対応 msw_chata_20201013
#ifdef COMPACT_MA
	UB			write_cnt_serial_mc;			/* シリアルFLASH書込み			*/
	UB			read_err_serial_mc;				/* シリアルFLASH読込み異常検知	*/
#endif	// COMPACT_MA
};
// 6201異常対策 msw_nagasi_20201005_end
/********	 室内機モード制限				********/
struct ic_mode_limit {
	UB			coolmode	:1;			/* 冷房モード制限				*/
	UB			heatmode	:1;			/* 暖房モード制限				*/
	UB			drymode		:1;			/* ドライモード制限				*/
	UB			automode	:1;			/* 自動モード制限				*/
	UB			fanmode		:1;			/* 風速モード制限				*/
	UB			ventmode	:1;			/* 換気モード制限				*/
	UB			fu_hum_mode	:1;			/* (外気処理)加湿運転モード制限	*/	//[32MA A01 osada]外気処理対応
	UB			fu_fan_hum	:1;			/* (外気処理)送風加湿モード制限	*/	//[32MA A01 osada]外気処理対応
	UB			fu_cool_hum	:1;			/* (外気処理)冷房加湿モード制限	*/	//[32MA A01 osada]外気処理対応
};

/********	 室内機機能対応					********/
struct ic_func_limit {
	UB			louver		:1;			/* ルーバー有無				*/
	UB			vane		:1;			/* ベーン有無				*/
	UB			swing		:1;			/* スイング有無				*/
	UB			lossnai		:1;			/* ロスナイ有無				*/
	UB			vaneauto	:1;			/* ベーン自動５段有無		*/	//[A_018 NS_KOYAMA 080620]
	UB 			vane_auto_mx	:1;     		/* ベーン自動有無 */
	UB 			vane_5_mx	:1;     		/* ベーン５段有無 */	
	UB			fanauto		:1;			/* 風速自動有無				*/	//[A_018 NS_KOYAMA 080620]
	
	UB			mente		:1;			/* メンテナンス対応			*/
	UB			coolhpburn	:2;			/* 冷専／ヒーポン／燃焼暖房	*/
	UB			automode	:1;			/* 自動有無					*/
	UB			dry			:1;			/* ドライ有無				*/
	UB			sensorpos	:1;			/* センサー位置				*/
	UB			extmode		:1;			/* 拡張機能対応				*/
	UB			vane1h		:1;			/* ベーン１時間有効			*/
	
	UB			tempdisp	:1;			/* 室内温度表示有無			*/
	UB			fanstep		:2;			/* 風速段数					*/
	UB			temprange	:3;			/* 設定温度範囲  MA用			*/
	
	UB			temprange_me	:3;			/* 設定温度範囲　ME用				*/
	UB			temppoint5	:1;			/* 温度表示単位(0.5℃表示)	*/
	UB			humfunc		:1;			/* 湿度対応					*/	//[A_023 NS_KOYAMA 080710]
	UB			hipower		:1;			/* 冷暖快速					*/	//[A_023 NS_KOYAMA 080710]
	UB			savefunc	:1;			/* 能力セーブ				*/	//[A_023 NS_KOYAMA 080710]
	UB			fu_hum					:1;	/* (外気処理)加湿機能			*/	//[32MA A01 osada]外気処理対応
	
	UB			fu_notch_pmt			:1;	/* (外気処理)強ノッチ操作制限	*/	//[32MA A01 osada]外気処理対応
	UB			fu_notch_sp				:1;	/* (外気処理)強ノッチ風量		*/	//[32MA A01 osada]外気処理対応
	UB			fu_night_parge_enable	:1;	/* (外気処理)ナイトパージ設定	*/	//[32MA A01 osada]外気処理対応
	UB			fu_24h_state_enable		:1;	/* (外気処理)24hr運転設定		*/	//[32MA A01 osada]外気処理対応
	UB			fu_mode_disp			:1;	/* (外気処理)運転ﾓｰﾄﾞ表示内容	*/	//[32MA A01 osada]外気処理対応
	UB			fu_settemp_disp			:1;	/* (外気処理)設定温度表示		*/	//[32MA A01 osada]外気処理対応
	UB			temp05_mb				:1;	/* (12K機種)0.5℃対応			*/	//[33MA A01g SEC Nanjo.Y. 20111224]
	UB			setback_mc		:1;			/* セットバック有無			*/		// 35MA 131203 chata B3コマンド対応・自動二値
	
	UB			auto2_mc		:1;			/* 自動二値有無				*/		// 35MA 131203 chata B3コマンド対応・自動二値
	UB			mode_exp_mc		:1;			/* モード・温度拡張対応(B3対応)	*/	// 35MA 131203 chata B3コマンド対応・設定温度範囲拡張
	UB			heat			:1;			/* 暖房モード有無	*/
	UB			fan			:1;			/* 送風モード有無	*/
	UB			auto_cool_heat_disp_mc	:1;	/* 自動冷暖房表示	*/
	UB			ic_type			:2;	/* 機種（FP、AIC、RAC HAC)	*/
	
	UB			louverLR		:2;			/* 左右風向有無(段数)			*/	// [40MA国内1st MSW Miyazaki]
	UB			night_perge_func_mb:1;					// (外気処理)ナイトパージ機能有無[0:未対応 / 1:対応]					//国内版44MA_換気設定(ナイトパージ機能)_MSW chata_20211020
	UB			vent_24hic_func_mb:1;					// (外気処理)24時間換気機能有無[0:未対応 / 1:対応]						//国内版44MA_換気設定(24時間換気機能)_MSW chata_20211020
	UB			vane_ateyoke_auto_mb:1;		// 風あてよけ自動機能有無[0:未対応 / 1:対応]										//国内版44MA_風あてよけ自動_MSW chata_20211026
	UB			diagnosis_mode_enable_mb:1;	// 診断運転モード対応状態[0:未対応 / 1:対応]										//国内版44MA_GL17対応_MSW inoue_20211125
	UB			vane_4steps_auto_mb:1;		// ベーン自動機能+4段 [0:vaneauto 依存 / 1:自動有り+4段]							//国内版45MA_上下風向4段自動対応_MESW_inoue_20221207
	UB			sv_alarm_mb		:1;			// 管理人室アラーム制御		// Kawa231013
	UB			ilock_fnc_mb	:1;			// インターロック機能		// Kawa231013
	UB			ilock_req_mb	:1;			// インターロック確認要求	// Kawa231013
};
struct vent {						/* 換気								*/
	UB 			power	:1;			/* 換気運転／停止					*/
	UB 			speed	:1;			/* 換気風量							*/
	UB 			interlock:1;		/* 連動								*/
};

/********	 操作禁止		********/
struct rclock {
	UB			power	:1;			/* 発停許可／禁止					*/
	UB			mode	:1;			/* 運転モード許可／禁止				*/
	UB			temp	:1;			/* 設定温度許可／禁止				*/
	UB			filter	:1;			/* フィルターリセット許可／禁止		*/
	UB			fansp_mc:1;			/* 風速操作許可/禁止				*/	// 35MA 131203 chata B3コマンド対応・操作禁止拡充
	UB			fan_mc	:1;			/* 風向操作許可/禁止				*/	// 35MA 131203 chata B3コマンド対応・操作禁止拡充
	UB			timer_mc:1;			/* タイマー動作許可/禁止			*/	// 35MA 131203 chata B3コマンド対応・操作禁止拡充
};
/*********	設定温度(拡張対応)	***************/
struct settemp_as {
	union temp	cool_mx;			/* 冷房設定温度			*/
	union temp	heat_mx;			/* 暖房設定温度			*/
	union temp	auto_mx;			/* 自動(一値)設定温度	*/
	union temp	upper_mx;			/* セットバック上限		*/
	union temp	lower_mx;			/* セットバック下限		*/
	UB 		override_mc;			/* オーバーライド中		*/
};
/*********	設定温度範囲(拡張対応)	***************/
struct range{
	union temp upper_mx;	//上限
	union temp lower_mx;	//下限
};
struct settemp_range_as {
	struct range cool_mx;
	struct range heat_mx;
	struct range auto_mx;
	struct range setback_upper_mx;
	struct range setback_lower_mx;		/* 2017/4/27 MSW ogawa #6588 */
};
struct operation_delay
{
	UB is_delay;
	UB timer_id;
	union value
	{
		UB power;
		UB mode;
		union temp temp;
		UB fanspeed;
		UB vane;
		UB louver;
		struct vent vent;
		UB louverLR;			// [40MA国内1st MSW Miyazaki]
		UB hummode;
		UB fu_vent;
	};
	union value value;
};
struct operation_delay_backup
{
	struct operation_delay onoff;		/*!<発停			*/
	struct operation_delay mode;		/*!<運転モード		*/
	struct operation_delay temp;		/*!<設定温度		*/
	struct operation_delay fansp;		/*!<風速			*/
	struct operation_delay vane;		/*!<風向			*/
	struct operation_delay louver;		/*!<ルーバー		*/
	struct operation_delay lossnay;		/*!<換気			*/
	struct operation_delay louverLR;	/*!<左右風向		*/	// [40MA国内1st MSW Miyazaki]
	struct operation_delay hummode;		/*!<加湿モード		*/
};

typedef struct ignore_receive_t{
	UB onoff_mx:1;		/* 運転停止  読み捨て/受信 */
	UB vent_mx:1;		/* 換気連動  読み捨て/受信 */
	UB vane_mx:1;		/* 風向受  読み捨て/受信 */
	UB wind_speed_mx:1;     /* 風速   読み捨て/受信 */
	UB mode_mx:1;		/* 運転モード 読み捨て/受信 */
	UB louver_mx:1;		/* ルーバ  読み捨て/受信 */
	UB temperature_mx:1;	/* 設定温度  読み捨て/受信 */
	UB louverLR_mx:1;	/* 左右風向受  読み捨て/受信 */	// [40MA国内1st MSW Miyazaki]
	UB hummode_mx:1;	/* 加湿モード  読み捨て/受信 */
}ignore_receive_t;

// Kawa181115 start
// 内蔵データフラッシュ対応
struct e2p_initial_status{
	UB request_mc;		// 初期化要求
	UB eeprom_mc:1;		// EEPROM全体
	UB common_mc:1;		// EEPROM共通部位1
	UB common2_mc:1;	// EEPROM共通部位2
#ifdef MODEL_MA
	UB ma_mc:1;			// EEPROM_MA部位
	UB flash_mc:1;		// 内蔵データフラッシュ
#endif
#ifdef MODEL_ME
	UB me_mc:1;			// EEPROM_ME部位
#endif
#ifdef COMPACT_MA
	UB serial_mc:1;		// シリアルフラッシュ
#else	// SMART_MA or SMART_ME
#endif	// COMPACT_MA
};
// Kawa181115 end

/********	サマータイム日時			********/
struct summertime_date{
	UB	week_mc;		/*!<曜日	*/
	UB	weeknumber_mc;	/*!<週		*/
	UB	month_mc;		/*!<月		*/
};

/********	サマータイム			********/
struct summertime{
	UB						summertime_execstate_mc;	/*!<サマータイム実施状態	*/
	UB						summertime_enable_mc;		/*!<サマータイム有効無効	*/
	struct summertime_date	start_date_mx;				/*!<開始日					*/
	struct time				s_before_time_mx;			/*!<開始変更前時間			*/
	struct time				s_after_time_mx;			/*!<開始変更後時間			*/
	struct summertime_date	end_date_mx;				/*!<終了日					*/
	struct time				e_before_time_mx;			/*!<終了変更前時間			*/
	struct time				e_after_time_mx;			/*!<終了変更後時間			*/
};

/********	Bluetooth接続情報			********/
struct ble_info{
	UH			pincode_mu;		/*!<PINコード				*/
	UB			id_mch[DSTTYPE_SIZE];	/*!<リモコン形名			*/
	UB			bdadr_mch[BLE_BD_ADDR_VAL_SIZE];	/*!<BDアドレス				*/
};

/********	Bluetooth設定			********/
struct ble_set{
	UB			bleset_mc;		/*!<Bluetooth設定			*/
	UB			pin_init_mc;	/*!<PINコード初期化フラグ	*/
	UH			pincode_mu;		/*!<PINコード				*/
	UB			autoset_mc;		/*!<PINコード自動更新設定	*/
	struct time	autotime_mx;	/*!<PINコード自動更新時刻	*/
};

/********	形名			********/
struct dsttype{
	UB			data_mch[DSTTYPE_SIZE];	/*!<データ					*/
};

/********	製造番号			********/
struct serialno{
	UB			data_mch[SERIALNO_SIZE];	/*!<データ					*/
};

/********	BDアドレス			********/
struct bdaddress{
	UB			data_mch[BLE_BD_ADDR_STR_SIZE];	/*!<データ					*/
};
/********	 BLEセンサ設定情報			********/										//国内版44MA_定国内版44MA_セントラルペリフェラルの自動切換え機能対応_chata_20211207
struct ble_senser_info{
	UB			ble_thermo_senser_enable_mc;		/*!<温度センサ使用			*/
	UB			ble_co2_senser_enable_mc;			/*!<CO2センサ使用			*/
	UH			ble_co2ppm_target_mu;				/*!<目標CO2濃度				*/
};


/********	 BLEセンサー受信状態		********/										//国内版44MA_定国内版44MA_セントラルペリフェラルの自動切換え機能対応_chata_20211207
struct ble_sensor_state_info {	
	UB			ble_sensor_state_mc;			/* 検知状態(0:NotFound 1:Found		*/
	union temp	ble_sensor_temp_mx;				/* 検知温度							*/
	UH			ble_sensor_co2ppm_mu;			/* 検知CO2濃度						*/
// ↓国内版45MA_無線環境センサの検知データ送信_Masuda_20221215
	UB			ble_sensor_pm10_mc;				/* 検知PM1.0 [ug/m3]				*/
	UB			ble_sensor_pm25_mc;				/* 検知PM2.5 [ug/m3]				*/
	UB			ble_sensor_humidity_mc;			/* 検知湿度  [%]					*/
// ↑国内版45MA_無線環境センサの検知データ送信_Masuda_20221215
};

// Kawa231013 start
/********	 回路検査実施時刻	********/
struct circuit_chktime{
	UB			enable_mb:1;	// 有効/無効
	UB			addr_mb:7;		// M-NETアドレス
	struct date	date_mx;		// 年月日
	struct time	time_mx;		// 時分
	UB		safetydev_chk_mc;	// 安全装置動作状態		//47MA SEC A.Shinbori 2024.10 仕様変更(G07改訂_アプリ)
};
// Kawa231013 end

//47MA SEC A.Shinbori 2024.10 不具合修正(#67425/#67563) ↓
// 回路検査状態
struct circuit_chk_state{
	UB		chk_state_mc;	// 回路検査状態
	UB		adr_mc;			// 検出元アドレス
};
//47MA SEC A.Shinbori 2024.10 不具合修正(#67425/#67563) ↑

//↓46MA 換気急速表示対応_MESW_tsuji_20231002
/********   換気動作設定詳細(スリム)   ********/ 
struct vent_setting_detail
{
	UB kisyu_select_mc;						/*!<機種選択			*/
	UB vent_quick_display_enable_mb:1;		/*!<換気急速表示		*/
	UB vent24h_enable_mb:1;					/*!<24時間換気			*/
	UB delay_drive_enable_mb:1;				/*!<遅延運転			*/
	UB in_people_gale_enable_mb:1;			/*!<在人強風			*/
	UB defrost_vent_speed_reduction_enable_mb:1;	/*!<霜取時換気風量抑制	*/	//47MA 霜取時換気風量抑制_MESW_Masuda_20241108
	UB absence_set_mc;						/*!<不在時設定			*/
};
//↑46MA 換気急速表示対応_MESW_tsuji_20231002

//↓46MA 換気機器選択対応_MESW_Masuda_20231020
/********	 換気制御IF切替情報			********/
/*この情報はスリム以外の機種では更新されないため、スリム機種でのみ使用すること*/
struct vent_ctrl_if_switch_info {
	UB	vent_ctrl_if_switch_IC_mc;			/* 換気制御IF切替 ICからの受信値 */
	UB	vent_ctrl_if_switch_RC_mc;			/* 換気制御IF切替 RC判定値	*/
	UB	vent_ctrl_if_ope_RC_mc;				/* 換気操作	RC判定値		*/
};
//↑46MA 換気機器選択対応_MESW_Masuda_20231020

//↓46MA 外部機器操作履歴対応_MESW_tsuji_20231121
struct external_ope_log{
	UB	storage_flg_mc;						/* 格納フラグ					*/
	struct date	date_mx;					/* xx年xx月xx日					*/
	struct time	time_mx;					/* 時刻xx時yy分					*/
};
//↑46MA 外部機器操作履歴対応_MESW_tsuji_20231121

#ifdef COMPACT_MA
/********	 デザイン設定情報			********/
struct design_set{
	UB			select_mc;		/*!<カラー選択				*/
	B			hueset_ms;		/*!<色相(設定)				*/
	UB			pattern_mc;		/*!<パターン				*/
	UB			application_mc;	/*!<カラー適用				*/
};

/********	 タッチパネル補正情報			********/
struct calibration_set{
	UH			data_muh[CALIBPOINT_SIZE][COORDINATE_SIZE];	/*!<データ					*/
};

/********	 ExRomIconリスト			********/
struct ex_romicon_list{
	UB			ena_flag_mc;	/*!<有効フラグ				*/
	UH			wide_mu;		/*!<幅						*/
	UH			high_mu;		/*!<高さ					*/
	UW			flash_addr_ml;	/*!<フラッシュアドレス		*/
	UB			pal_id_mc;		/*!<パレットID				*/
};

/********	 ExRomIconサイズ			********/
struct ex_romicon_size{
	UH			wide_mu;		/*!<幅						*/
	UH			high_mu;		/*!<高さ					*/
};

/********	 ExRomIconデータ			********/
struct ex_romicon_data{
	UH			size_mu;		/*!<サイズ					*/
	UB			*data_mcp;		/*!<データ格納場所			*/
};

/********	 Exパレットリスト			********/
struct ex_pallet_list{
	UB			ena_flag_mc;	/*!<有効フラグ				*/
	UB			col_num_mc;		/*!<色bit数					*/
	UB			rom_id_mc;		/*!<基準パレットID			*/
	UB			ram_id_mc;		/*!<RAMパレットID			*/
	UW			flash_addr_ml;	/*!<フラッシュアドレス		*/
};

/********	 Exパレットデータ			********/
struct ex_pallet_data8{
	UH			data_muh[PALLET_SIZE_8];	/*!<データ					*/
};

/********	 タッチパネル補正データセット			********/
struct calibration_dataset{
	UB calibflag_mc;			/*!<タッチパネル補正有無	*/
	struct calibration_set calibdata_mx;	/*!<データ					*/
};

#endif	// COMPACT_MA

#if defined(ROM_MAA) || defined(ROM_MAAC)
/********	 ナイトセットバック設定			//SEC 2016.06.01 sugimoto 国内外統合	********/
struct night_setback_setting {
	UB			enable;				/* 有効／無効				*/
	union  temp temp_upper;			/* 設定温度上限				*/
	union  temp temp_lower;			/* 設定温度下限				*/
	struct time	start;				/* 開始時刻					*/
	struct time	stop;				/* 終了時刻					*/
};

/********	 ナイトセットバック用記憶情報	//SEC 2016.06.01 sugimoto 国内外統合	********/
struct setback_memory {
	UB			unitmode;			/* 運転モード				*/
	union  temp	temp;				/* 設定温度					*/
	UB			ventpower;			/* 換気運転	[A_02 honda 100712] */
//	struct vent vent;				/* 換気	[30MAA A10 100927 sugimoto] */	//[30MAA A15 101018 sugimoto]
	union  temp	temp2;				/* 設定温度(自動二値時の下限)	*/		// ishika120823 B3コマンド対応・設定温度範囲拡張
	struct settemp_as  settemp;		/* モード毎設定温度	*/					//<20120927_watanabe_冷.86>
};
#endif	// ROM_MAA or ROM_MAAC

/*!< 揮発メモリ構造 - 共通部	*/
struct db_ram_common
{
	UB								rcstate;				/*!<リモコン状態			*/
	struct clock					clock;					/*!<時計					*/
	struct clock					clocknew;				/*!<時計更新時間			*/
	UB								testtimer;				/*!<試運転ﾀｲﾏ				*/
	UB								demomode;				/*!<デモモード				*/
	struct operation_delay_backup	delay;					/*!<猶予情報				*/
	struct error 					error_mx;				/*!<異常情報				*/
	struct rc_jp					mode_jp;				/*!<動作モード判定			*/
	struct e2p_initial_status		e2p_initial_status;		/*!<不揮発初期化中状態管理	*/
	UB								bleupstatus_mc;			/*!<BLE更新状態				*/
	UB								quickmode_mc;			/*!<時短モード				*/
	UB								disp_gui_id_mc;			/*!<表示中画面ID			*/		// 国内版45MA_画面ID取得方法変更_MESW_inoue_20230111
	struct err_dtt_cnt				err_dtt_cnt_mx;			/*!<エラー応答受信検知カウンタ	*/	// 6201異常対策 msw_nagasi_20201005
#ifdef COMPACT_MA
	struct ex_romicon_list			delicon_mxh[ROMICON_LIST_NUM];	/*!<ExRomIcon削除リスト		*/
	struct ex_pallet_list			delpallet_mxh[PALLET_LIST_NUM];	/*!<Exパレット削除リスト	*/
#else	// SMART_MA or SMART_ME
#endif	// COMPACT_MA
#if defined(ROM_MAA) || defined(ROM_MAAC)
	struct	setback_memory			setback_memory_ex;		/*!< ナイトセットバック用記憶情報	*/
	UB								setback_drive_mc;		/*!< ナイトセットバック運転状態	*/
#endif	// ROM_MAA or ROM_MAAC
	UB								bletermchk_mc;			/*!<BLE端末確認要求			*/		// [BLE] 20170330 Miyazaki
	UB								ucBle_MenteExeResult_mc;							/*!<BLEメンテナンス収集結果           */
	UB								ucBle_MenteExeSequenceKind_mc;						/*!<BLEメンテナンス収集シーケンス種別 */
	UB								ucBle_MenteExitTimer_mc;							/*!<BLEメンテナンス終了タイマ状態 */
	UB								ucBle_FncselExeResult_mc;							/*!<BLE機能選択 中断有無           */
	UW								ble_reset_24timer_ml;								/*!<BLE_RESET_24hrタイマ           */	//国内版42MA_定期的RL78リセット機能対応_chata_20190826
	struct clock					retryclock;											/*!<時計更新時間			*///msw_chata_20200818_HWリトライ対応 //msw_nagasi_20201005
	UB								vent_interlock_state_mc;							/*!<換気連動状態			*/	// 国内版44MA_CO2換気連動_MSW inoue_20211220
#ifdef USE_SE
	UB								se_id_mc;				/*!<設備PAC中温機種の設定温度下限値切換識別	*/	//45MA-SE_設定温度範囲変更_MESW_Masuda_20230629
#endif /* USE_SE */
// Kawa221219 start
	UB 								buzzer_state_mc;									// ブザー鳴動状態
// Kawa221219 end
	UB								defrost_vent_speed_reduction_control_state_mc;		/*!<霜取時換気風量抑制状態 */ //47MA 霜取時換気風量抑制_MESW_Masuda_20241108
};

/*!< 不揮発メモリ構造 - 共通部	*/
/* データを増やす場合は次の領域(MA_E2P_ADDR)へはみ出さないように気を付けること。 */
/* 共通部の不揮発領域は2回に分けてEEPROMへ書き込むが、その間に96ﾊﾞｲﾄ隙間がある為、次の領域の先頭から96減じた位置が上限となる。 */
struct db_nv_common
{
	UB								dispmode;											/*!<メイン画面表示形式		*/
	UB								isclockset;											/*!<時計設定状態			*/
	struct opr_lock					oprlock;											/*!<操作ロック				*/
	struct onoff_timer				onoff_timer;										/*!<オンオフタイマ			*/
	struct autooff_timer			autooff_timer;										/*!<消し忘れ防止タイマ		*/
	UB								enable_weekschedule;								/*!<週間スケジュール有効無効	*/
	struct common_weekly_schedule	common_weekly_schedule_mx[SCHEDULE_NUM_MAX][WEEK];	/*!<週間スケジュール設定情報	*/
	struct temp_limit				temp_limit;											/*!<設定温度範囲制限		*/
	struct temp_auto_return			temp_auto_return;									/*!<設定温度自動復帰		*/
	UB								rc_name[RCNAME_LEN + 1];							/*!<リモコン名称			*/
	UB								telno[TELNAME_LEN + 1];								/*!<電話番号設定			*/
	B								contrast;											/*!<コントラスト			*/
	struct rc_disp					rc_disp;											/*!<リモコン表示設定		*/
	struct icoc_info				icoc_info[MAX_REIBAI];								/*!<室内機室外機型名情報	*/
	struct service_info				service_info;										/*!<サービス情報			*/
	struct err_log					err_log[ERRLOG_MAX];								/*!<異常履歴				*/
	UH								adm_password;										/*!<管理者パスワード		*/
	UH								mente_password;										/*!<メンテナンスパスワード	*/
	H								offset_temp_mw;										/*!<室内温度補正値			*/
	UB								f4_longpush_enable;									/*!<F4ボタン長押し有効無効	*/
	UB								language_type;										/*!<言語種別				*/
	struct summertime				summertime_mx;										/*!<サマータイム情報		*/
	struct ble_set					bleset_mx;											/*!<Bluetooth設定			*/
	UB								brightness_mc;										/*!<輝度					*/
	UB								device_name_mch[BLE_DEV_NAME_BODY_LEN + 1];			/*!<デバイス名称			*/
	struct bdaddress				bdadr_mx;											/*!<BDアドレス				*/
#ifdef COMPACT_MA
	UB								iconexplan_mc;										/*!<アイコン説明			*/
	UB								allmode_mc;											/*!<常灯モード				*/
	struct design_set				designset_mx;										/*!<デザイン設定情報		*/
	struct ex_romicon_list			romicon_mxh[ROMICON_LIST_NUM];						/*!<ExRomIconリスト			*/
	struct ex_pallet_list			pallet_mxh[PALLET_LIST_NUM];						/*!<Exパレットリスト		*/
	struct calibration_dataset		calibdataset_mx;									/*!<タッチパネル補正データセット	*/
#else	// SMART_MA or SMART_ME
	UB								inversion_mc;										/*!<白黒反転表示			*/
#endif	// COMPACT_MA
#if defined(ROM_MAA) || defined(ROM_MAAC)
	struct night_setback_setting	night_setback_setting;								/*!<ナイトセットバック設定	*/
#endif	// ROM_MAA or ROM_MAAC
#ifdef ROM_MAAU
	UB								hold_state_mc;										/*!<ホールド状態			*/
	UB								setbackmode_mc;										/*!<セットバックモード		*/
#endif	// ROM_MAAU
	struct gra_err_log				grace_err_log_mxh[GRACE_ERRLOG_MAX];				/*!<異常猶予履歴			*/	// 国内版43MA_異常猶予履歴対応_inoue_20200924
	struct bdaddress				wlst_bdadr_mx;										/*!<WhiteList登録BDアドレス	*/	//44MA セントラルペリフェラルの自動切換え機能対応_msw chata_20211207
	struct ble_senser_info			ble_senser_info_mx;									/*!<BLEセンサ設定情報		*/	//44MA セントラルペリフェラルの自動切換え機能対応_msw chata_20211207
	UB								vent_stop_setting_mc;								/*!<空調機停止時の換気停止	*/	//国内版45MA_空調換気画面対応_MESW_Masuda_20230120
	UB 								buzzer_level_mc;									/*!<ブザー音量			*/
// Kawa231013 start
//	struct circuit_chktime			circuit_chktime_mx;									// 回路検査実施時刻		//47MA SEC A.Shinbori 2024.10 仕様変更(G07改訂_アプリ)
	struct circuit_chktime			circuit_chktime_mx[CIRCUIT_CHKTIME_MAX];			// 回路検査実施時刻		//47MA SEC A.Shinbori 2024.10 仕様変更(G07改訂_アプリ)
// Kawa231013 end
	struct vent_setting_detail		vent_setting_detail_mx;								/*!<換気動作設定詳細		*/	//国内版46MA_換気急速表示対応_MESW_tsuji_20231002
	struct external_ope_log			external_ope_log_mxh[EXTERNAL_OPE_LOG_MAX];			/*!<外部機器操作履歴対応	*/	//46MA 外部機器操作履歴対応_MESW_tsuji_20231121
	UB								rc_alarm_setting_mc;								/*!<リモコンアラーム設定	*/		//アプリ通信対応_202409_SEC kashiwagi
};

/*!< 不揮発メモリ構造 - 出荷検査部	*/
struct db_nv_shipment
{
#ifdef COMPACT_MA
	struct calibration_set			shipcalib_mx;										/*!<タッチパネル出荷補正情報	*/
#else	// SMART_MA or SMART_ME
#endif	// COMPACT_MA
	struct serialno					serialno_mx;										/*!<製造番号				*/
	struct bdaddress				bdadr_mx;											/*!<BDアドレス				*/
};

// 配管温度の変換テーブル用
typedef struct {
	B	c_mc;
	H	f_ml;
}TEMP_TBL_PIPING;

/*
 * --------------------------------------------------------------------------
 *   External Variable Define Area
 * --------------------------------------------------------------------------
 */
extern const UB temp_tbl_ech[TEMP_TBL_MAX][2];			// 35MA 131203 chata  B3コマンド対応・自動二値機能
extern const TEMP_TBL_PIPING temp_tbl_piping_exh[TEMP_TBL_PIPING_MAX];
extern const struct db_nv_common db_nv_common_initvalue;

/*
 * --------------------------------------------------------------------------
 *   Prototype Declaration Area
 * --------------------------------------------------------------------------
 */
UB init_db_ram_common(void);			/*!<揮発データベースの初期化		*/
UB init_db_nv_common(void);				/*!<不揮発データベースの初期化		*/
UB init_db_nv_shipment_fc(void);		/*!<出荷検査データベースの初期化	*/
UB factory_default_db_nv_common(void);	/*!<不揮発データを出荷時状態に戻す	*/
#ifdef COMPACT_MA
UB factory_logo_db_nv_common(void);		/*!<不揮発データのロゴ画像をONにする*/
#else	// SMART_MA or SMART_ME
#endif	// COMPACT_MA
UB dbinfo_ramini_demo_fc(void);			/*!<デモモード時RAMデータ初期化		*/

/****************************************************************************
 *	S/Wバージョン
 ****************************************************************************/
UB read_db_swver_fc(UH *para1);

/****************************************************************************
 *	量産管理バージョン
 ****************************************************************************/
UB read_db_mgver_fc(UH *para1);

/****************************************************************************
 *	リモコン形名
 ****************************************************************************/
UB read_db_dsttype_fc(struct dsttype *para1_rxp);

/****************************************************************************
 *	リモコン状態
 ****************************************************************************/
UB read_db_rcstate_fc(UB *para1_rcp);
UB write_db_rcstate_fc(UB *para1_rcp); 
 
/****************************************************************************
 *	時計
 ****************************************************************************/
UB read_db_clock_fc(struct clock *para1_rxp);
UB write_db_clock_fc(struct clock *para1_rxp);
UB read_db_clocknew_fc(struct clock *para1_rxp);
UB write_db_clocknew_fc(struct clock *para1_rxp);
UB read_db_retryclock_fc(struct clock *para1_rxp);		//msw_chata_20200818_HWリトライ対応 //msw_nagasi_20201005
UB write_db_retryclock_fc(struct clock *para1_rxp);		//msw_chata_20200818_HWリトライ対応 //msw_nagasi_20201005

/****************************************************************************
 *	リモコンジャンパー
 ****************************************************************************/
//UB read_db_rc_jp_fc(struct rc_jp *para1_rxp);
//UB write_db_rc_jp_fc(struct rc_jp *para1_rxp);
 
/****************************************************************************
 *	試運転タイマ													//＜[A_018 NSCS_KABASAWA 080617]>
 ****************************************************************************/
UB read_db_testtimer_fc(UB *para1_rcp);
UB write_db_testtimer_fc(UB *para1_rcp);

/*****************************************************************
 *	デモ機能
 *****************************************************************/
UB read_db_demomode_fc(UB *para1_rcp);
UB write_db_demomode_fc(UB *para1_rcp);

/*****************************************************************
 *	猶予情報
 *****************************************************************/ 
UB read_db_delay_fc(struct operation_delay_backup *para1_rxp);
UB write_db_delay_fc(struct operation_delay_backup *para1_rxp);
 
/*****************************************************************
 *	メイン画面表示形式
 *****************************************************************/  
UB read_db_dispmode_fc(UB *para1_rcp);
UB write_db_dispmode_fc(UB *para1_rcp);

/*****************************************************************
 *	時計設定状態
 *****************************************************************/  
UB read_db_isclockset_fc(UB *para1_rcp);
UB write_db_isclockset_fc(UB *para1_rcp);

/*****************************************************************
 *	操作ﾛｯｸ
 *****************************************************************/  
UB read_db_oprlock_fc(struct opr_lock *para1_rxp);
UB write_db_oprlock_fc(struct opr_lock *para1_rxp);
 
/*****************************************************************
 *	ｵﾝｵﾌﾀｲﾏ読み出し
 *****************************************************************/
UB read_db_onoff_timer_fc(struct onoff_timer *para1_rxp);
UB write_db_onoff_timer_fc(struct onoff_timer *para1_rxp);

/*****************************************************************
 *	消し忘れ防止タイマ
 *****************************************************************/
UB read_db_autooff_timer_fc(struct autooff_timer *para1_rxp);
UB write_db_autooff_timer_fc(struct autooff_timer *para1_rxp);

/*****************************************************************
 *	週間ｽｹｼﾞｭｰﾙ 有効無効
 *****************************************************************/
UB read_db_enable_weekschedule_fc(UB *para1_rcp);
UB write_db_enable_weekschedule_fc(UB *para1_rcp);

/*****************************************************************
 *	週間ｽｹｼﾞｭｰﾙ設定情報
 *****************************************************************/
UB read_db_aweek_schedule_fc(UB select_rc, UB n_rc, struct common_weekly_schedule *para1_rxp);
UB write_db_aweek_schedule_fc(UB select_rc, UB n_rc, struct common_weekly_schedule *para1_rxp);

/*****************************************************************
 *	設定温度範囲制限
 *****************************************************************/
UB read_db_temp_limit_fc(struct temp_limit *para1_rxp);
UB write_db_temp_limit_fc(struct temp_limit *para1_rxp);

/*****************************************************************
 *	設定温度自動復帰
 *****************************************************************/
UB read_db_temp_auto_return_fc(struct temp_auto_return *para1_rxp);
UB write_db_temp_auto_return_fc(struct temp_auto_return *para1_rxp);

/*****************************************************************
 *	リモコン名称
 *****************************************************************/
UB read_db_rcname_fc(UB *para1_rcp);
UB write_db_rcname_fc(UB *para1_rcp);

/*****************************************************************
 *	電話番号
 *****************************************************************/
UB read_db_telno_fc(UB *para1_rcp);
UB write_db_telno_fc(UB *para1_rcp);
 
/*****************************************************************
 *	コントラスト
 *****************************************************************/ 
UB read_db_contrast_fc(B *para1_rsp);
UB write_db_contrast_fc(B *para1_rsp);

/*****************************************************************
 *	リモコン表示設定
 *****************************************************************/ 
UB read_db_rcdisp_fc(struct rc_disp *para1_rxp);
UB write_db_rcdisp_fc(struct rc_disp *para1_rxp);

/*****************************************************************
 *	室内機型名情報
 *****************************************************************/ 
UB read_db_icoc_info_fc(UB n_rc, struct icoc_info *para1_rxp);
UB write_db_icoc_info_fc(UB n_rc, struct icoc_info *para1_rxp);
/* 下記は型名、製造番号、型名コードを同時にwriteすると					*/
/* サイズオーバーとなるため、3つに分割する(GUI→APLの設定時のみ使用)	*/
UB write_db_icoc_info_name_fc(UB n_rc, struct icoc_info_name *para1_rxp);
UB write_db_icoc_info_serial_fc(UB n_rc, struct icoc_info_serial *para1_rxp);
UB write_db_icoc_info_namecode_fc(UB n_rc, struct icoc_info_namecode *para1_rxp);

/*****************************************************************
 *	サービス情報
 *****************************************************************/ 
UB read_db_service_info_fc(struct service_info *para1_rxp);
UB write_db_service_info_fc(struct service_info *para1_rxp);

/*****************************************************************
 *	異常履歴
 *****************************************************************/ 
UB read_db_err_log_fc(UB n_rc, struct err_log *para1_rxp);
UB write_db_err_log_fc(UB n_rc, struct err_log *para1_rxp);

// ↓国内版43MA_異常猶予履歴対応_inoue_20200924
/*****************************************************************
 *	異常猶予履歴
 *****************************************************************/ 
UB read_db_grace_err_log_fc(UB n_rc, struct gra_err_log *para1_rxp);
UB write_db_grace_err_log_fc(UB n_rc, struct gra_err_log *para1_rxp);
// ↑国内版43MA_異常猶予履歴対応_inoue_20200924

/*****************************************************************
 *	管理者パスワード
 *****************************************************************/ 
UB read_db_adm_password_fc(UH *para1_rup);
UB write_db_adm_password_fc(UH *para1_rup);

/*****************************************************************
 *	メンテナンスパスワード
 *****************************************************************/ 
UB read_db_mente_password_fc(UH *para1_rup);
UB write_db_mente_password_fc(UH *para1_rup);

/*****************************************************************
 *	室内温度補正値
 *****************************************************************/ 
UB read_db_offset_temp_fc(H *para1_rwp);
UB write_db_offset_temp_fc(H *para1_rwp);

/*****************************************************************
 *	F4ボタン長押し有効無効
 *****************************************************************/ 
UB read_db_f4_longpush_enable_fc(UB *para1_rcp);
UB write_db_f4_longpush_enable_fc(UB *para1_rcp);

/*****************************************************************
 *	異常情報
 *****************************************************************/ 
UB read_db_error_fc(struct error *para1_rcp);
UB write_db_error_fc(struct error *para1_rcp);

// 6201異常対策 msw_nagasi_20201005_start
/*****************************************************************
 *	エラー応答受信検知カウンタ
 *****************************************************************/ 
UB read_db_errdttcnt_fc(struct err_dtt_cnt *para1_rcp);
UB write_db_errdttcnt_fc(struct err_dtt_cnt *para1_rcp);
// 6201異常対策 msw_nagasi_20201005_end

/*****************************************************************
 *	EEPROM初期化状態
 *****************************************************************/ 
UB read_db_eeprom_initial_status_fc(struct e2p_initial_status *para1_rcp);
UB write_db_eeprom_initial_status_fc(struct e2p_initial_status *para1_rcp);

/*****************************************************************
 *	動作モード判定JP
 *****************************************************************/ 
UB read_db_modejp_fc(struct rc_jp *para1_rcp);
UB write_db_modejp_fc(struct rc_jp *para1_rcp);

#ifdef USE_SE												//↓45MA-SE_設定温度範囲変更_MESW_Masuda_20230629
/*****************************************************************
 *	設定温度下限値切換識別JP
 *****************************************************************/ 
UB read_db_SE_Board_ID_fc(UB *para1_rcp);
UB write_db_SE_Board_ID_fc(UB *para1_rcp);
#endif /* USE_SE */											//↑45MA-SE_設定温度範囲変更_MESW_Masuda_20230629

/*****************************************************************
 *	言語種別
 *****************************************************************/ 
UB read_db_language_type(UB *para1_rcp);
UB write_db_language_type(UB *para1_rcp);

/*****************************************************************
 *	サマータイム情報
 *****************************************************************/ 
UB read_db_summertime_fc(struct summertime *para1_rxp);
UB write_db_summertime_fc(struct summertime *para1_rxp);

/*****************************************************************
 *	Bluetooth設定
 *****************************************************************/ 
UB read_db_bleset_fc(struct ble_set *para1_rxp);
UB write_db_bleset_fc(struct ble_set *para1_rxp);

/*****************************************************************
 *	輝度設定
 *****************************************************************/ 
UB read_db_brightness_fc(UB *para1_rcp);
UB write_db_brightness_fc(UB *para1_rcp);

/*****************************************************************
 *	BLE更新状態
 *****************************************************************/ 
UB read_db_bleupstatus_fc(UB *para1_rcp);
UB write_db_bleupstatus_fc(UB *para1_rcp);

/*****************************************************************
 *	時短モード
 *****************************************************************/ 
UB read_db_quickmode_fc(UB *para1_rcp);
UB write_db_quickmode_fc(UB *para1_rcp);

/*****************************************************************
 *	表示中画面ID									// 国内版45MA_画面ID取得方法変更_MESW_inoue_20230111
 *****************************************************************/ 
UB read_db_disp_gui_id_fc(UB *para1_rcp);
UB write_db_disp_gui_id_fc(UB *para1_rcp);

/*****************************************************************
 *	BLE端末確認要求
 *****************************************************************/ 
UB read_db_bletermchk_fc(UB *para1_rcp);			// [BLE] 20170330 Miyazaki
UB write_db_bletermchk_fc(UB *para1_rcp);			// [BLE] 20170330 Miyazaki

#ifdef COMPACT_MA
/*****************************************************************
 *	アイコン説明
 *****************************************************************/ 
UB read_db_iconexplan_fc(UB *para1_rcp);
UB write_db_iconexplan_fc(UB *para1_rcp);

/*****************************************************************
 *	常灯モード
 *****************************************************************/ 
UB read_db_allmode_fc(UB *para1_rcp);
UB write_db_allmode_fc(UB *para1_rcp);

/*****************************************************************
 *	デザイン設定情報
 *****************************************************************/ 
UB read_db_designset_fc(struct design_set *para1_rxp);
UB write_db_designset_fc(struct design_set *para1_rxp);

/*****************************************************************
 *	ExRomIconリスト
 *****************************************************************/ 
UB read_db_ex_romicon_list_fc(UB n_rc, struct ex_romicon_list *para1_rxp);
UB write_db_ex_romicon_list_fc(UB n_rc, struct ex_romicon_list *para1_rxp);

/*****************************************************************
 *	ExRomIcon削除リスト
 *****************************************************************/ 
UB read_db_ex_delicon_list_fc(UB n_rc, struct ex_romicon_list *para1_rxp);
UB write_db_ex_delicon_list_fc(UB n_rc, struct ex_romicon_list *para1_rxp);

/*****************************************************************
 *	Exパレットリスト
 *****************************************************************/ 
UB read_db_ex_pallet_list_fc(UB n_rc, struct ex_pallet_list *para1_rxp);
UB write_db_ex_pallet_list_fc(UB n_rc, struct ex_pallet_list *para1_rxp);

/*****************************************************************
 *	Exパレット削除リスト
 *****************************************************************/ 
UB read_db_ex_delpallet_list_fc(UB n_rc, struct ex_pallet_list *para1_rxp);
UB write_db_ex_delpallet_list_fc(UB n_rc, struct ex_pallet_list *para1_rxp);

/*****************************************************************
 *	タッチパネル補正データセット
 *****************************************************************/ 
UB read_db_calibdataset_fc(struct calibration_dataset *para1_rxp);
UB write_db_calibdataset_fc(struct calibration_dataset *para1_rxp);
#else	// SMART_MA or SMART_ME

/*****************************************************************
 *	白黒反転表示
 *****************************************************************/ 
UB read_db_inversion_fc(UB *para1_rcp);
UB write_db_inversion_fc(UB *para1_rcp);
#endif	// COMPACT_MA

#if defined(ROM_MAA) || defined(ROM_MAAC)
/*****************************************************************
 *	ナイトセットバック設定
 *****************************************************************/ 
UB read_db_night_setback_setting_fc(struct night_setback_setting *para1_rcp);
UB write_db_night_setback_setting_fc(struct night_setback_setting *para1_rcp);
/*****************************************************************
 *	ナイトセットバック運転情報記憶書き込み
 *****************************************************************/ 
UB read_db_setback_memory_fc(struct setback_memory *para1_rcp);
UB write_db_setback_memory_fc(struct setback_memory *para1_rcp);
/*****************************************************************
 *	ナイトセットバック運転状態
 *****************************************************************/ 
UB read_db_setback_drive_fc(UB *para1_rcp);
UB write_db_setback_drive_fc(UB *para1_rcp);
#endif	// ROM_MAA or ROM_MAAC

#ifdef ROM_MAAU
/****************************************************************************
 *	ホールド状態
 ****************************************************************************/
UB read_db_hold_status(UB *para1_rcp);
UB write_db_hold_status(UB *para1_rcp);

/****************************************************************************
 *	セットバックモード
 ****************************************************************************/
UB read_db_setbackmode_fc(UB *para1_rcp);
UB write_db_setbackmode_fc(UB *para1_rcp);
#endif	// ROM_MAAU

/****************************************************************************
 *	空調機停止時の換気停止 設定
 ****************************************************************************/
UB read_db_vent_stop_setting_fc(UB *para1_rcp);			//国内版45MA_空調換気画面対応_MESW_Masuda_20230120
UB write_db_vent_stop_setting_fc(UB *para1_rcp);		//国内版45MA_空調換気画面対応_MESW_Masuda_20230120

#ifdef COMPACT_MA
/****************************************************************************
 *	タッチパネル出荷補正情報
 ****************************************************************************/
UB read_db_shipcalib_fc(struct calibration_set *para1_rxp);
#else	// SMART_MA or SMART_ME
#endif	// COMPACT_MA

/****************************************************************************
 *	製造番号
 ****************************************************************************/
UB read_db_serialno_fc(struct serialno *para1_rxp);

/****************************************************************************
 *	BDアドレス
 ****************************************************************************/
UB read_db_bdaddress_fc(struct bdaddress *para1_rxp);
UB write_db_bdaddress_fc(struct bdaddress *para1_rcp);

/****************************************************************************
 *	デバイス名称
 ****************************************************************************/
UB read_db_devicename_fc(UB *para1_rcp);
UB write_db_devicename_fc(UB *para1_rcp);

/****************************************************************************
 *	ロゴ画像処理状態
 ****************************************************************************/
UB read_db_blelogo_status_fc(UB *para1_rcp);
UB write_db_blelogo_status_fc(UB *para1_rcp);
/****************************************************************************
 *	BLEメンテナンス収集結果
 ****************************************************************************/
UB read_db_BleMenteExeResult_fc(UB *ptBleMenteExeResult_rcp);
UB write_db_BleMenteExeResult_fc(UB ucBleMenteExeResult_rc);

/****************************************************************************
 *	BLEメンテナンス収集結果
 ****************************************************************************/
UB read_db_BleMenteExeSequenceKind_fc(UB *ptBleMenteExeSequenceKind_rcp);
UB write_db_BleMenteExeSequenceKind_fc(UB ucBleMenteExeSequenceKind_rc);

/****************************************************************************
 *	BLEメンテナンス終了タイマ状態
 ****************************************************************************/
UB read_db_BleMenteExitTimerSts_fc(UB *ptBleMenteExitTimerSts_rcp);
UB write_db_BleMenteExitTimerSts_fc(UB ucBleMenteExitTimerSts_rc);
/****************************************************************************
 *	BLE機能選択 中断有無
 ****************************************************************************/
UB read_db_BleFncselExeResult_fc(UB *ptBleFncselExeResult_rcp);
UB write_db_BleFncselExeResult_fc(UB ucBleFncselExeResult_rc);

/****************************************************************************
*	BLE_RESET_24hrタイマ 		//国内版42MA_定期的RL78リセット機能対応_chata_20190826
 ****************************************************************************/
UB read_db_ble_reset_24timer_fc(UW *para1_rcp);
UB write_db_ble_reset_24timer_fc(UW *para1_rcp);
/****************************************************************************
 *	WhiteList登録BDアドレス					//44MA セントラルペリフェラルの自動切換え機能対応_msw chata_20211207
 ****************************************************************************/
UB read_db_wlst_bdaddress_fc(struct bdaddress *para1_rxp);
UB write_db_wlst_bdaddress_fc(struct bdaddress *para1_rcp);

/****************************************************************************
 *	BLEセンサ設定情報						//44MA セントラルペリフェラルの自動切換え機能対応_msw chata_20211207
 ****************************************************************************/
UB read_db_ble_senser_info_fc(struct ble_senser_info *para1_rxp);
UB write_db_ble_senser_info_fc(struct ble_senser_info *para1_rxp);

/****************************************************************************
 *	CO2換気連動								// 国内版44MA_CO2換気連動_MSW inoue_20211220
 ****************************************************************************/
UB read_db_vent_interlock_state_fc(UB *para1_rcp);
UB write_db_vent_interlock_state_fc(UB *para1_rcp);

/****************************************************************************
 *	ブザー音量
 ****************************************************************************/
UB read_db_buzzer_level_fc(UB *para1_rcp);
UB write_db_buzzer_level_fc(UB *para1_rcp);

/****************************************************************************
 *	リモコンアラーム設定				//アプリ通信対応_202409_SEC kashiwagi
 ****************************************************************************/
UB read_db_rc_alarm_setting_fc(UB *para1_rcp);
UB write_db_rc_alarm_setting_fc(UB *para1_rcp);

// Kawa221219 start
/****************************************************************************
 *	ブザー鳴動状態
 ****************************************************************************/
UB read_db_buzzer_state_fc(UB *para1_rcp);
UB write_db_buzzer_state_fc(UB *para1_rcp);
// Kawa221219 end

// Kawa231013 start
/****************************************************************************
 *	回路検査実施時刻
 ****************************************************************************/
//47MA SEC A.Shinbori 2024.10 仕様変更(G07改訂_アプリ) ↓
//UB read_db_circuit_chktime_fc(struct circuit_chktime *para1_rxp);
//UB write_db_circuit_chktime_fc(struct circuit_chktime *para1_rxp);
UB read_db_circuit_chktime_fc(UB n_rc, struct circuit_chktime *para1_rxp);
UB write_db_circuit_chktime_fc(UB n_rc, struct circuit_chktime *para1_rxp);
//47MA SEC A.Shinbori 2024.10 仕様変更(G07改訂_アプリ) ↑
// Kawa231013 end

/*****************************************************************
 *	換気急速表示							// 46MA 換気急速表示対応_MESW_tsuji_20231002
 *****************************************************************/
UB read_db_vent_setting_detail_fc(struct vent_setting_detail *para1_rxp);
UB write_db_vent_setting_detail_fc(struct vent_setting_detail *para1_rxp);

/*****************************************************************
 *	外部機器操作履歴						// 46MA 外部機器操作履歴対応_MESW_tsuji_20231121
 *****************************************************************/
UB read_db_external_ope_log_fc(UB n_rc, struct external_ope_log *para1_rxp);
UB write_db_external_ope_log_fc(UB n_rc, struct external_ope_log *para1_rxp);

/****************************************************************************
 *	霜取時換気風量抑制						//47MA 霜取時換気風量抑制_MESW_Masuda_20241108
 ****************************************************************************/
UB read_db_defrost_vent_speed_reduction_control_fc(UB *para1_rcp);
UB write_db_defrost_vent_speed_reduction_control_fc(UB *para1_rcp);

#endif	/*	INCLUDE_APL_DB_COMMON_H	*/
