#if 0
// debug function
static void writecoefs(const int_fast32_t * lCoeff, int iCoefNum)
{
	debug_printf_P(PSTR("# iCoefNum = %i\n"), iCoefNum);
	const int iHalfLen = (iCoefNum - 1) / 2;
	int i;
	for (i = 0; i <= iHalfLen; ++ i)
	{
		debug_printf_P(PSTR("%ld\n"), lCoeff [i]);
	}
	i -= 1;
	for (; -- i >= 0; )
	{
		debug_printf_P(PSTR("%ld\n"), lCoeff [i]);
	}
	debug_printf_P(PSTR("# end\n"));
}
#endif

/*-----------------------------------*/

// float32 * float32 -> float32
static RAMFUNC FLOAT32P_t scalepair(FLOAT32P_t a, FLOAT_t b)
{
	a.IV *= b;
	a.QV *= b;
	return a;
}

#if 0

// int32 * int32 -> int32
static RAMFUNC FLOAT32P_t scalepair_int32(INT32P_t a, int_fast32_t b)
{
	FLOAT32P_t v;
	const int_fast64_t bi = b;
	v.IV = (a.IV * bi) >> 31;
	v.QV = (a.QV * bi) >> 31;
	return v;
}
#endif

#if WITHDSPLOCALFIR
#if WITHDSPEXTDDC
// ������ ������������ ������� ��������
// ������������ � ������ �������� DDCV
static RAMFUNC_NONILINE FLOAT32P_t filter_firp_rx_SSB_IQ(FLOAT32P_t NewSample)
{
	const FLOAT_t * const k = FIRCoef_rx_SSB_IQ [gwprof];
	enum { Ntap = Ntap_rx_SSB_IQ, NtapHalf = Ntap / 2 };
	// ����� � ������������ ���������� �������
	static FLOAT32P_t x [Ntap * 2] = { { { 0, 0 }, }, };
	static uint_fast16_t fir_head = 0;

	// shift the old samples
	// fir_head -  ������ �������������� ����� ������
	// fir_head + Ntap -  ������� �� ������ ��������������� �����
	fir_head = (fir_head == 0) ? (Ntap - 1) : (fir_head - 1);
   	x [fir_head] = x [fir_head + Ntap] = NewSample;

	uint_fast16_t bh = fir_head + NtapHalf;			// ������ �������������� ����� ������
	uint_fast16_t bt = bh;	// ������� �� ������ ��������������� ������
   	// Calculate the new output
	uint_fast16_t n = NtapHalf;
	// ������� � �������� ������
	FLOAT32P_t v = scalepair(x [bh], k [n]);            // sample at middle of buffer
	do
	{	
		{
			const FLOAT_t kv = k [-- n];
			v.IV += kv * (x [-- bh].IV + x [++ bt].IV);
			v.QV += kv * (x [bh].QV + x [bt].QV);
		}
		{
			const FLOAT_t kv = k [-- n];
			v.IV += kv * (x [-- bh].IV + x [++ bt].IV);
			v.QV += kv * (x [bh].QV + x [bt].QV);
		}
	}
	while (n != 0);

    return v;
}
// ������ ������������ ������� �����������
// ������������ � ������ �������� DUC
static RAMFUNC_NONILINE FLOAT32P_t filter_firp_tx_SSB_IQ(FLOAT32P_t NewSample)
{
	const FLOAT_t * const k = FIRCoef_tx_SSB_IQ [gwprof];
	enum { Ntap = Ntap_tx_SSB_IQ, NtapHalf = Ntap / 2 };
	// ����� � ������������ ���������� �������
	static FLOAT32P_t x [Ntap * 2] = { { { 0, 0 }, }, };
	static uint_fast16_t fir_head = 0;

	// shift the old samples
	// fir_head -  ������ �������������� ����� ������
	// fir_head + Ntap -  ������� �� ������ ��������������� �����
	fir_head = (fir_head == 0) ? (Ntap - 1) : (fir_head - 1);
    x [fir_head] = x [fir_head + Ntap] = NewSample;

	uint_fast16_t bh = fir_head + NtapHalf;			// ������ �������������� ����� ������
	uint_fast16_t bt = bh;	// ������� �� ������ ��������������� ������
    // Calculate the new output
	uint_fast16_t n = NtapHalf;
	// ������� � �������� ������
	FLOAT32P_t v = scalepair(x [bh], k [n]);            // sample at middle of buffer
	do
	{	
		{
			const FLOAT_t kv = k [-- n];
			v.IV += kv * (x [-- bh].IV + x [++ bt].IV);
			v.QV += kv * (x [bh].QV + x [bt].QV);
		}
		{
			const FLOAT_t kv = k [-- n];
			v.IV += kv * (x [-- bh].IV + x [++ bt].IV);
			v.QV += kv * (x [bh].QV + x [bt].QV);
		}
	}
	while (n != 0);

    return v;
}

#else  /* WITHDSPEXTDDC */
// �������
// ������ (floating point) ��� ������������ ��������. ����������� ��� multiplier 0 1 0 -1.
// Speed tests: DT=145 (129), ntaps=481

static RAMFUNC_NONILINE FLOAT32P_t filter_fir4_rx_SSB_IQ(FLOAT32P_t NewSample, uint_fast8_t useI) 
{
	const FLOAT_t * const k = FIRCoef_rx_SSB_IQ [gwprof];
	enum { Ntap = Ntap_rx_SSB_IQ, NtapHalf = Ntap / 2 };
	FLOAT32P_t rv;

	// ����� � ������������ ���������� �������
	static FLOAT32P_t x [Ntap * 2] = { { { 0, 0, }, }, }; // input samples (force CCM allocation)
	static uint_fast16_t fir_head = 0;		// ������� ������ � ����� � ��������� ���

	// shift the old samples
	// fir_head -  ������ �������������� ����� ������
	// fir_head + Ntap -  ������� �� ������ ��������������� �����
	fir_head = (fir_head == 0) ? (Ntap - 1) : (fir_head - 1);
	const uint_fast16_t fir_tail = fir_head + Ntap;
	// Calculate the new output

	uint_fast16_t n;
	if (useI != 0)
	{
		// ���� ������� == 0 - ��� � ��� ����� ���� ����������
		x [fir_head].IV = NewSample.IV;
		x [fir_tail].IV = NewSample.IV;
		//
		// ������������ I
		//
		// ������� � �������� ������
		rv.IV = k [NtapHalf] * x [fir_head + NtapHalf].IV;
		uint_fast16_t bh = fir_head;	// ������ �������������� ����� ������
		uint_fast16_t bt = fir_tail;	// ������� �� ������ ��������������� ������
		// NewSample.QV ����� ����� 0 - ����� �� �����������
		n = 0;
		do
		{	
			// ��������� I
			// 1_23 * 1_31 = 1_54. ��� ����� ��������� �� FIRCoef.
			rv.IV += k [n ++] * (x [bh ++].IV + x [-- bt].IV);
			// ���������� Q
			++ n;
			++ bh;
			-- bt;
		}
		while (n < NtapHalf);
		//
		// ������������ Q
		//
		rv.QV = 0;
		bh = fir_head;			// ������ �������������� ����� ������
		bt = fir_tail;			// ������� �� ������ ��������������� ������
		n = 0;
		do
		{	
			// ���������� I
			++ n;
			++ bh;
			-- bt;
			// ��������� Q
			// 1_23 * 1_31 = 1_54. ��� ����� ��������� �� FIRCoef.
			rv.QV += k [n ++] * (x [bh ++].QV + x [-- bt].QV);
		}
		while (n < NtapHalf);
	}
	else
	{
		// ���� ������� == 0 - ��� � ��� ����� ���� ����������
		x [fir_head].QV = NewSample.QV;
		x [fir_tail].QV = NewSample.QV;
		//
		// ������������ Q
		//
		// ������� � �������� ������
		rv.QV = k [NtapHalf] * x [fir_head + NtapHalf].QV;
		uint_fast16_t bh = fir_head;	// ������ �������������� ����� ������
		uint_fast16_t bt = fir_tail;	// ������� �� ������ ��������������� ������
		// NewSample.IV ����� ����� 0 - ����� �� �����������
		n = 0;
		do
		{	
			// ��������� Q
			// 1_23 * 1_31 = 1_54. ��� ����� ��������� �� FIRCoef.
			rv.QV += k [n ++] * (x [bh ++].QV + x [-- bt].QV);
			// ���������� I
			++ n;
			++ bh;
			-- bt;
		}
		while (n < NtapHalf);
		//
		// ������������ I
		//
		rv.IV = 0;
		bh = fir_head;			// ������ �������������� ����� ������
		bt = fir_tail;			// ������� �� ������ ��������������� ������
		n = 0;
		do
		{	
			// ���������� Q
			++ n;
			++ bh;
			-- bt;
			// ��������� I
			// 1_23 * 1_31 = 1_54. ��� ����� ��������� �� FIRCoef.
			rv.IV += k [n ++] * (x [bh ++].IV + x [-- bt].IV);
		}
		while (n < NtapHalf);
	}
    return rv;
}

// ������ ������������ ������� �����������. ����������� ��� multiplier 0 1 0 -1.
// ���������� ���������� �� ���, � � ����������� �� useI
static RAMFUNC_NONILINE FLOAT32P_t filter_fir4_tx_SSB_IQ(FLOAT32P_t NewSample, uint_fast8_t useI) 
{
	const FLOAT_t * const k = FIRCoef_tx_SSB_IQ [gwprof];
	enum { Ntap = Ntap_tx_SSB_IQ, NtapHalf = Ntap / 2 };
	// ����� � ������������ ���������� �������
	static FLOAT32P_t x [Ntap * 2] = { { { 0, 0, }, }, }; // input samples (force CCM allocation)
	static uint_fast16_t fir_head = 0;

	// shift the old samples
	// fir_head -  ������ �������������� ����� ������
	// fir_head + Ntap -  ������� �� ������ ��������������� �����
	fir_head = (fir_head == 0) ? (Ntap - 1) : (fir_head - 1);

	// ���� ������� == 0 - ��� � ��� ����� ���� ����������
	uint_fast16_t bh = fir_head;			// ������ �������������� ����� ������
	uint_fast16_t bt = fir_head + Ntap;	// ������� �� ������ ��������������� ������
    // Calculate the new output
    x [bh] = NewSample;
    x [bt] = NewSample;
	uint_fast16_t n = 0;
	FLOAT32P_t v = { { 0, 0 }, };
	if (useI != 0)
	{
		// �� ������ ���������� ������ I
		do
		{	
			v.IV += k [n ++] * (x [bh ++].IV + x [-- bt].IV);
			v.IV += k [n ++] * (x [bh ++].IV + x [-- bt].IV);
		}
		while (n < NtapHalf);
		// ������� � �������� ������
		v.IV += k [NtapHalf] * x [bh].IV;
	}
	else
	{
		// �� ������ ���������� ������ Q
		do
		{	
			v.QV += k [n ++] * (x [bh ++].QV + x [-- bt].QV);
			v.QV += k [n ++] * (x [bh ++].QV + x [-- bt].QV);
		}
		while (n < NtapHalf);
		// ������� � �������� ������
		v.QV += k [NtapHalf] * x [bh].QV;
	}

    return v;
}
#endif /* WITHDSPEXTDDC */

#endif /* WITHDSPLOCALFIR */

/* ���������� ��������� � ������������ FIR ������� */
static RAMFUNC FLOAT_t filter_fir_compute(const FLOAT_t * const pk0, const FLOAT_t * xbh, uint_fast16_t n)
{
	const FLOAT_t * xbt = xbh;		// ������� ������ �� ������
    // Calculate the new output
	// ������� � �������� ������
	FLOAT_t v = pk0 [-- n] * * -- xbh;             // sample at middle of buffer
#if __ARM_NEON_FP && DSP_FLOAT_BITSMANTISSA == 24
	float32x4_t v4 = vdupq_n_f32(0);
#endif /* __ARM_NEON_FP */
	do
	{	
#if __ARM_NEON_FP && DSP_FLOAT_BITSMANTISSA == 24
		// ������� �������� �� ����� ��������
		xbh -= 4;
		const float32x4_t vh = vld1q_f32(xbh);
		// �������� �������� �� ������ �������� (��������� ���������� ������� �� ���������������)
		const float32x4_t vt = vld1q_f32(xbt);
		xbt += 4;
		/* ��������� ����� � ������ */
		const float32x4_t va = vaddq_f32(
			vrev64q_f32(vcombine_f32(vget_high_f32(vt), vget_low_f32(vt))), // ������ ������� �� ���������������
			vh);
		/* ������������ */
		n -= 4;
		const float32x4_t vk = vld1q_f32(& pk0 [n]);
		/* ��������� � ����������� */
		v4 = vmlaq_f32(v4, va, vk);
#elif defined (FMAF)
		v = FMAF(pk0 [-- n], * -- xbh + * xbt ++, v);	/* ���������� ���������� ��� ������ �������� ��-�� ���������� ����� ��������� */
#else /* defined (FMAF) */
		v += pk0 [-- n] * (* -- xbh + * xbt ++);
#endif /* defined (FMAF) */
	}
	while (n != 0);
#if __ARM_NEON_FP && DSP_FLOAT_BITSMANTISSA == 24
	v += vgetq_lane_f32(v4, 0);
	v += vgetq_lane_f32(v4, 1);
	v += vgetq_lane_f32(v4, 2);
	v += vgetq_lane_f32(v4, 3);
#endif /* __ARM_NEON_FP */

	return v;
}

#if WITHUSEDUALWATCH && DUALFILTERSPROCESSING

/* ���������� ��������� � ������������ FIR ������� */
// ���������� ��� �������� ���������� ��������
static RAMFUNC_NONILINE FLOAT32P_t filter_fir_compute_Pair(const FLOAT_t * const pk0, const FLOAT32P_t * xbh, uint_fast16_t n)
{
	const FLOAT32P_t * xbt = xbh;		// ������� ������ �� ������
    // Calculate the new output
	// ������� � �������� ������
	const FLOAT32P_t t1 = * -- xbh;
	const FLOAT_t k = pk0 [-- n];
	FLOAT32P_t v = { { k * t1.IV, k * t1.QV } };             // sample at middle of buffer
#if __ARM_NEON_FP && DSP_FLOAT_BITSMANTISSA == 24
	float32x4_t v4 = vdupq_n_f32(0);	// lane 0, 2, IV values, lane 1, 3: QV values
#endif /* __ARM_NEON_FP */
	do
	{	
#if __ARM_NEON_FP && DSP_FLOAT_BITSMANTISSA == 24
		// ������� �������� �� ����� ��������
		xbh -= 2;
		const float32x4_t vh = vcombine_f32(vld1_f32(xbh [0].ivqv), vld1_f32(xbh [1].ivqv));	// lane 0, 2, IV values, lane 1, 3: QV values
		// ������� �������� �� ������ �������� (� �������� ������� - �������� � ��� �� �������, ��� � ������������)
		const float32x4_t vt = vcombine_f32(vld1_f32(xbt [1].ivqv), vld1_f32(xbt [0].ivqv));	// lane 0, 2, IV values, lane 1, 3: QV values
		xbt += 2;
		/* ��������� ����� � ������ */
		const float32x4_t va = vaddq_f32(
			vt,
			vh);
		/* ������������ */
		n -= 2;
		const float32x4_t vk = vcombine_f32(vdup_n_f32(pk0 [n + 0]), vdup_n_f32(pk0 [n + 1]));	// lane 0, 2: IV values, lane 1, 3: QV values
		/* ��������� � ����������� */
		v4 = vmlaq_f32(v4, va, vk);
#elif defined (FMAF)
		const FLOAT32P_t t1 = * -- xbh;
		const FLOAT32P_t t2 = * xbt ++;
		const FLOAT_t k = pk0 [-- n];
		v.IV = FMAF(k, t1.IV + t2.IV, v.IV);	/* ���������� ���������� ��� ������ �������� ��-�� ���������� ����� ��������� */
		v.QV = FMAF(k, t1.QV + t2.QV, v.QV);	/* ���������� ���������� ��� ������ �������� ��-�� ���������� ����� ��������� */
#else /* defined (FMAF) */
		const FLOAT32P_t t1 = * -- xbh;
		const FLOAT32P_t t2 = * xbt ++;
		const FLOAT_t k = pk0 [-- n];
		v.IV += k * (t1.IV + t2.IV);
		v.QV += k * (t1.QV + t2.QV);
#endif /* defined (FMAF) */
	}
	while (n != 0);
#if __ARM_NEON_FP && DSP_FLOAT_BITSMANTISSA == 24
	v.IV += vgetq_lane_f32(v4, 0);	// IV values
	v.IV += vgetq_lane_f32(v4, 2);	// IV values
	v.QV += vgetq_lane_f32(v4, 1);	// QV values
	v.QV += vgetq_lane_f32(v4, 3);	// QV values
#endif /* __ARM_NEON_FP */

	return v;
}


/* ���������� ��������� � ������������ FIR ������� */
// ���������� ��� �������� ������� ���������
static RAMFUNC_NONILINE FLOAT32P_t filter_fir_compute_Pair2(const FLOAT_t * const pk0, const FLOAT_t * const pk1, const FLOAT32P_t * xbh, uint_fast16_t n)
{
	const FLOAT32P_t * xbt = xbh;		// ������� ������ �� ������
    // Calculate the new output
	// ������� � �������� ������
	const FLOAT32P_t t1 = * -- xbh;
	const FLOAT_t k0 = pk0 [-- n];
	const FLOAT_t k1 = pk1 [n];
	FLOAT32P_t v = { { k0 * t1.IV, k1 * t1.QV } };             // sample at middle of buffer
#if __ARM_NEON_FP && DSP_FLOAT_BITSMANTISSA == 24
	float32x4_t v4 = vdupq_n_f32(0);	// lane 0, 2, IV values, lane 1, 3: QV values
#endif /* __ARM_NEON_FP */
	do
	{	
#if __ARM_NEON_FP && DSP_FLOAT_BITSMANTISSA == 24
		// ������� �������� �� ����� ��������
		xbh -= 2;
		const float32x4_t vh = vcombine_f32(vld1_f32(xbh [0].ivqv), vld1_f32(xbh [1].ivqv));	// lane 0, 2, IV values, lane 1, 3: QV values
		// ������� �������� �� ������ �������� (� �������� ������� - �������� � ��� �� �������, ��� � ������������)
		const float32x4_t vt = vcombine_f32(vld1_f32(xbt [1].ivqv), vld1_f32(xbt [0].ivqv));	// lane 0, 2, IV values, lane 1, 3: QV values
		xbt += 2;
		/* ��������� ����� � ������ */
		const float32x4_t va = vaddq_f32(
			vt,
			vh);
		/* ������������ */
		n -= 2;
		const float32_t vks [4] = { pk0 [n + 0], pk1 [n + 0], pk0 [n + 1], pk1 [n + 1] };
		//const float32x4_t vk = vld1q_f32(vls);
		/* ��������� � ����������� */
		v4 = vmlaq_f32(v4, va, vld1q_f32(vks));
#elif defined (FMAF)
		const FLOAT32P_t t1 = * -- xbh;
		const FLOAT32P_t t2 = * xbt ++;
		const FLOAT_t k0 = pk0 [-- n];
		const FLOAT_t k1 = pk1 [n];
		v.IV = FMAF(k0, t1.IV + t2.IV, v.IV);	/* ���������� ���������� ��� ������ �������� ��-�� ���������� ����� ��������� */
		v.QV = FMAF(k1, t1.QV + t2.QV, v.QV);	/* ���������� ���������� ��� ������ �������� ��-�� ���������� ����� ��������� */
#else /* defined (FMAF) */
		const FLOAT32P_t t1 = * -- xbh;
		const FLOAT32P_t t2 = * xbt ++;
		const FLOAT_t k0 = pk0 [-- n];
		const FLOAT_t k1 = pk1 [n];
		v.IV += k0 * (t1.IV + t2.IV);
		v.QV += k1 * (t1.QV + t2.QV);
#endif /* defined (FMAF) */
	}
	while (n != 0);
#if __ARM_NEON_FP && DSP_FLOAT_BITSMANTISSA == 24
	v.IV += vgetq_lane_f32(v4, 0);	// IV values
	v.IV += vgetq_lane_f32(v4, 2);	// IV values
	v.QV += vgetq_lane_f32(v4, 1);	// QV values
	v.QV += vgetq_lane_f32(v4, 3);	// QV values
#endif /* __ARM_NEON_FP */

	return v;
}

#endif /* DUALFILTERSPROCESSING */

// ������ ��������� �����������
static RAMFUNC_NONILINE FLOAT_t filter_fir_tx_MIKE(FLOAT_t NewSample, uint_fast8_t bypass) 
{
	enum { Ntap = Ntap_tx_MIKE, NtapHalf = Ntap / 2 };
	// ����� � ������������ ���������� �������
	static FLOAT_t xshift [Ntap * 2] = { 0, };
	static uint_fast16_t fir_head = 0;

	// shift the old samples
	fir_head = (fir_head == 0) ? (Ntap - 1) : (fir_head - 1);
    xshift [fir_head] = xshift [fir_head + Ntap] = NewSample;

	return bypass ? xshift [fir_head + NtapHalf] : filter_fir_compute(FIRCoef_tx_MIKE [gwprof], & xshift [fir_head + NtapHalf + 1], NtapHalf + 1);
}

// �������� ������ ��������.
static RAMFUNC_NONILINE FLOAT_t filter_fir_rx_AUDIO_A(FLOAT_t NewSample) 
{
	enum { Ntap = Ntap_rx_AUDIO, NtapHalf = Ntap / 2 };
	// ����� � ������������ ���������� �������
	static FLOAT_t xshift [Ntap * 2] = { 0, };
	static uint_fast16_t fir_head = 0;

	// shift the old samples
	fir_head = (fir_head == 0) ? (Ntap - 1) : (fir_head - 1);
    xshift [fir_head] = xshift [fir_head + Ntap] = NewSample;

	return filter_fir_compute(FIRCoef_rx_AUDIO [gwprof] [0], & xshift [fir_head + NtapHalf + 1], NtapHalf + 1);
}

#if WITHUSEDUALWATCH

// �������� ������ ��������.
static RAMFUNC_NONILINE FLOAT_t filter_fir_rx_AUDIO_B(FLOAT_t NewSample) 
{
#if ! DUALFILTERSPROCESSING
	return NewSample;
#endif /* ! DUALFILTERSPROCESSING */

	enum { Ntap = Ntap_rx_AUDIO, NtapHalf = Ntap / 2 };
	// ����� � ������������ ���������� �������
	static FLOAT_t xshift [Ntap * 2] = { 0, };
	static uint_fast16_t fir_head = 0;

	// shift the old samples
	fir_head = (fir_head == 0) ? (Ntap - 1) : (fir_head - 1);
    xshift [fir_head] = xshift [fir_head + Ntap] = NewSample;

	return filter_fir_compute(FIRCoef_rx_AUDIO [gwprof] [1], & xshift [fir_head + NtapHalf + 1], NtapHalf + 1);
}

#if DUALFILTERSPROCESSING
// �������� ������ ��������.
// ���������� ��� �������� ���������� ��������
static RAMFUNC_NONILINE FLOAT32P_t filter_fir_rx_AUDIO_Pair(FLOAT32P_t NewSample) 
{
	enum { Ntap = Ntap_rx_AUDIO, NtapHalf = Ntap / 2 };
	// ����� � ������������ ���������� �������
	static FLOAT32P_t xshift [Ntap * 2] = { 0, };
	static uint_fast16_t fir_head = 0;

	// shift the old samples
	fir_head = (fir_head == 0) ? (Ntap - 1) : (fir_head - 1);
    xshift [fir_head] = xshift [fir_head + Ntap] = NewSample;

	return filter_fir_compute_Pair(FIRCoef_rx_AUDIO [gwprof] [0], & xshift [fir_head + NtapHalf + 1], NtapHalf + 1);
}

// �������� ������ ��������.
// ���������� ��� �������� ������� ���������
static RAMFUNC_NONILINE FLOAT32P_t filter_fir_rx_AUDIO_Pair2(FLOAT32P_t NewSample) 
{
	enum { Ntap = Ntap_rx_AUDIO, NtapHalf = Ntap / 2 };
	// ����� � ������������ ���������� �������
	static FLOAT32P_t xshift [Ntap * 2] = { 0, };
	static uint_fast16_t fir_head = 0;

	// shift the old samples
	fir_head = (fir_head == 0) ? (Ntap - 1) : (fir_head - 1);
    xshift [fir_head] = xshift [fir_head + Ntap] = NewSample;

	return filter_fir_compute_Pair2(FIRCoef_rx_AUDIO [gwprof] [0], FIRCoef_rx_AUDIO [gwprof] [1], & xshift [fir_head + NtapHalf + 1], NtapHalf + 1);
}
#endif /* DUALFILTERSPROCESSING */

#endif /* WITHUSEDUALWATCH */


static RAMFUNC uint_fast8_t isdspmoderx(uint_fast8_t dspmode)
{
	return dspmode > DSPCTL_MODE_RX_BASE && dspmode < DSPCTL_MODE_RX_TOP;
}

static RAMFUNC uint_fast8_t isdspmodetx(uint_fast8_t dspmode)
{
	return dspmode > DSPCTL_MODE_TX_BASE && dspmode < DSPCTL_MODE_TX_TOP;
}

