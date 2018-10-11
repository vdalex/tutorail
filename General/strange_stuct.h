static const sys_fsm_transition_t m_transition_table[] =
{
    SYS_FSM_STATE     ( S_WORKING ),
    SYS_FSM_TRANSITION( E_SET_DONE,          SYS_FSM_ALWAYS,            A_CONFIG_COMPLETE,             SYS_FSM_SAME_STATE ),
    SYS_FSM_TRANSITION( E_UART_TX_DONE,      GU_UART_TX_DATA_AVAILABLE, A_UART_TX_START,               SYS_FSM_SAME_STATE ),
    SYS_FSM_TRANSITION( E_UART_TX_DONE,      SYS_FSM_OTHERWISE,         A_UART_TX_IDLE_SET,            SYS_FSM_SAME_STATE ),
    SYS_FSM_TRANSITION( E_UART_RX_DONE,      GU_RADIO_TX_IDLE,          A_RADIO_TX_START,              SYS_FSM_SAME_STATE ),
    SYS_FSM_TRANSITION( E_RADIO_RX_DONE,     GU_UART_TX_IDLE,           A_UART_TX_START,               SYS_FSM_SAME_STATE ),
    SYS_FSM_TRANSITION( E_RADIO_TX_DONE,     GU_SEND_SUCCESS,           A_RADIO_TX_START,              SYS_FSM_SAME_STATE ),
    SYS_FSM_TRANSITION( E_RADIO_TX_DONE,     SYS_FSM_OTHERWISE,         A_RADIO_TX_RESTART,            SYS_FSM_SAME_STATE ),

    SYS_FSM_STATE     ( S_INITIAL ),
    SYS_FSM_TRANSITION( E_START_CONFIG,      SYS_FSM_ALWAYS,            A_CHANNEL_SET,                 S_SETTING_CH ),

    SYS_FSM_STATE     ( S_SETTING_CH ),
    SYS_FSM_TRANSITION( E_SET_DONE,          GU_SET_SUCCESS,            A_SHORT_ADDR_SET,              S_SETTING_SHORT_ADDR ),

    SYS_FSM_STATE     ( S_SETTING_SHORT_ADDR ),
    SYS_FSM_TRANSITION( E_SET_DONE,          GU_SET_SUCCESS,            A_EXT_ADDR_SET,                S_SETTING_EXT_ADDR ),

    SYS_FSM_STATE     ( S_SETTING_EXT_ADDR ),
    SYS_FSM_TRANSITION( E_SET_DONE,          GU_SET_SUCCESS,            A_PAN_ID_SET,                  S_SETTING_PAN_ID ),
#if (CONFIG_SECURE == 1)
    SYS_FSM_STATE     ( S_SETTING_PAN_ID ),
    SYS_FSM_TRANSITION( E_SET_DONE,          GU_SET_SUCCESS,            A_RX_ON_IDLE_SET,              S_SETTING_RX_ON_IDLE ),

    SYS_FSM_STATE     ( S_SETTING_RX_ON_IDLE ),
    SYS_FSM_TRANSITION( E_SET_DONE,          GU_SET_SUCCESS,            A_SECURITY_SET,                S_WORKING ),
#else
    SYS_FSM_STATE     ( S_SETTING_PAN_ID ),
    SYS_FSM_TRANSITION( E_SET_DONE,          GU_SET_SUCCESS,            A_RX_ON_IDLE_SET,              S_WORKING ),
#endif
};

/**@brief   Fixed-size type for FSM state ID.
 */
typedef uint8_t sys_fsm_state_id_t;


/**@brief   Fixed-size type for FSM event ID.
 */
typedef uint8_t sys_fsm_event_id_t;


/**@brief   Fixed-size type for FSM guard condition ID.
 */
typedef uint8_t sys_fsm_guard_id_t;


/**@brief   Fixed-size type for FSM action ID.
 */
typedef uint8_t sys_fsm_action_id_t;


/**@brief   FSM transition description (item of FSM transition table).
 *
 * @details When an event with given event_id occurs, the guard condition with guard_id
 *          is checked, and if it returns true, the action with action_id is performed,
 *          and state machine is switched to the state with new_state_id.
 */
typedef struct
{
    sys_fsm_event_id_t	event_id;        /**< FSM event ID. */
    sys_fsm_guard_id_t	guard_id;        /**< FSM guard ID. */
    sys_fsm_action_id_t	action_id;       /**< FSM action ID. */
    sys_fsm_state_id_t	new_state_id;    /**< New state ID. */
#if defined(CONFIG_FSM_DEBUG)
    const char * debug_string;
#endif
} sys_fsm_transition_t;

/**@brief    FSM state declaration.
 *
 * @details  The state is an aggregator item of the FSM transition table, aggregating
 *           the transitions, declared immediately after this state declaration.
 *           All transition declaration items, following the state declaration item,
 *           will be aggregated in this state, until the next state declaration item,
 *           or the "end of table" item.
 */
#define SYS_FSM_STATE(state_id)    \
            {(state_id) | SYS_FSM_STATE_FLAG, 0, 0, 0}

/**@brief   FSM transition declaration (item of FSM transition table).
 */
#if defined(CONFIG_FSM_DEBUG)
#   define SYS_FSM_TRANSITION(event_id, guard_id, action_id, new_state_id)	\
		{(event_id), (guard_id), (action_id), (new_state_id),   \
            "(" #event_id ", " #guard_id ", " #action_id " -> " #new_state_id ")"}
#else
#   define SYS_FSM_TRANSITION(event_id, guard_id, action_id, new_state_id)	\
		{(event_id), (guard_id), (action_id), (new_state_id)}
#endif

