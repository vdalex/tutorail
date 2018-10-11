static RAMFUNC FLOAT32P_t scalepair(FLOAT32P_t a, FLOAT_t b)
{
	a.IV *= b;
	a.QV *= b;
	return a;
}

static RAMFUNC_NONILINE FLOAT32P_t filter_firp_rx_SSB_IQ(FLOAT32P_t NewSample)
{
	const FLOAT_t * const k = FIRCoef_rx_SSB_IQ [gwprof];
	enum 
	{ 
		Ntap = Ntap_rx_SSB_IQ, 
		NtapHalf = Ntap / 2 
	};
	static FLOAT32P_t x [Ntap * 2] = { { { 0, 0 }, }, };
	static uint_fast16_t fir_head = 0;

	// shift the old samples
	// fir_head -  Íà÷àëî îáðàáàòûâàåìîé ÷àñòè áóôåðà
	// fir_head + Ntap -  Ïîçèöèÿ çà êîíöîì îáðàáàòûâàåìîãî áóôåð
	fir_head = (fir_head == 0) ? (Ntap - 1) : (fir_head - 1);
   	x [fir_head] = x [fir_head + Ntap] = NewSample;

	uint_fast16_t bh = fir_head + NtapHalf;		
	uint_fast16_t bt = bh;	
   	// Calculate the new output
	uint_fast16_t n = NtapHalf;
	
	FLOAT32P_t v = scalepair(x [bh], k [n]);     // sample at middle of buffer
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
