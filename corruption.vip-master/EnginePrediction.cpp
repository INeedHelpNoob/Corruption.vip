#include "EnginePrediction.h"
#include "checksum_md5.h"


std::unique_ptr<PredictionSystem> predictionSystem = std::make_unique<PredictionSystem>();
#define strenc( s ) ( s )
void PredictionSystem::start(CUserCmd *userCMD)
{

	IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (!pLocal) {
		return;
	}

	static bool predictionRandomSeedInit = false;
	//if (!predictionRandomSeedInit)
	//{
	//predictionRandomSeed = *(int**)GameUtils::FindPattern1(strenc("client.dll"), strenc("83 3D ? ? ? ? ? 74 1E 51") + 0x2);
	//predictionRandomSeedInit = true;

	//p_Console->ConsoleColorPrintf(Color::LightBlue(), "0x%x", predictionRandomSeed);
	//}

	//*predictionRandomSeed = MD5_PseudoRandom(userCMD->command_number) & 0x7FFFFFFF;

	oldCurtime = Interfaces::Globals->curtime;
	oldFrametime = Interfaces::Globals->frametime;

	Interfaces::Globals->curtime = pLocal->GetTickBase() * Interfaces::Globals->interval_per_tick;
	Interfaces::Globals->frametime = Interfaces::Globals->interval_per_tick;

	//Interfaces::GameMovement->StartTrackPredictionErrors(pLocal);

	memset(&moveData, 0, sizeof(moveData));
	Interfaces::MoveHelper->SetHost(pLocal);

	//p_MoveHelper->SetHost(reinterpret_cast<void*>(Global::locPlayer));
	Interfaces::Prediction1->SetupMove(pLocal, userCMD, Interfaces::MoveHelper, &moveData);

	//p_Prediction->SetupMove(reinterpret_cast<void*>(Global::locPlayer), userCMD, Interfaces::MoveHelper, &moveData);
	Interfaces::GameMovement->ProcessMovement(pLocal, &moveData);
	Interfaces::Prediction1->FinishMove(pLocal, userCMD, &moveData);
}

void PredictionSystem::end()
{

	IClientEntity *pLocal = Interfaces::EntList->GetClientEntity(Interfaces::Engine->GetLocalPlayer());

	if (!pLocal) {
		return;
	}

	//Interfaces::GameMovement->FinishTrackPredictionErrors(pLocal);
	Interfaces::MoveHelper->SetHost(0);

	*predictionRandomSeed = -1;

	Interfaces::Globals->curtime = oldCurtime;
	Interfaces::Globals->frametime = oldFrametime;
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class dujlhph {
public:
	bool atnjbmaktgkmz;
	dujlhph();
	int yurvgaqnibsmovzblnnxowq(bool xfpalxnav, bool oodujmhifedcv, bool mohgyidzozgvxk, double sznvfpfxhm, double pwvvyqzwdpmgkl, string fydlermzinllugj, bool jeayvyqvdao);
	double knygkfmhqkhqtgqo(int dhjeentecfr, string algtyyq, double ftvzel, double udlnmuwbjkork, string acmuob);
	void mcbetlukkbvo(double ozbvosvnawj, int betfouertoiydp, int keggzrmm, string mmwatptluc, bool qgmsaicp, bool rlcdblczpx, int azzat, int rknzifxqvokq, bool shfxtgxdnfh);
	double izqglkvxnppoapmouasneaiu(int btvvhanelvplxp, string ziidmytwhcet, bool izsatltxasgx, bool xstok, string uvwnrlpuklo, int vuqzjwwnq, int zyocd);
	bool aroqkhpwjxcejfhcdzhlo(bool uwtctbtmlhaj, int mjznxisffauoe, bool lhelwqveathyzuk, string gislnto, int apujlyg, bool shhaxfsagjo, double szmowaoizpeyzje, bool vpcwlyczxxe, int vwmjprsc);
	void uuoicgxtzkseoi(string lkglextvayl, string mybhfa, double nozhzon);

protected:
	string imupepycol;
	int ntuykuafpwizzlh;
	int cmviw;
	int zwrqfwndj;
	string xxujaii;

	string klfkqxyvqhh(bool jrggdip, bool fsqbahqjuzuhrs, double fvzrlnmyulz, string hwivqbicivwk);
	double aqiwzqamztobommomenyszc(bool yhqsiv, bool gyyxq, int aeyyvcwqozetqry, double bisnjzgg, double rdyproxnnew);
	bool ehmyjixfzq(string itmytj, string frehryzgv, int refpvuhb, string lnvlqvoin, double ughaih, string ksrgxtahsoislkv, int wquijfdlfohirx, int fxrgznnsiomia);
	string caynwmtrbpykbudrfn(bool iwfhloroqgrrf, int upodhhtuk, bool omvdfzjwlzrmmk, double jalvmjhdyst, int looehgn, double wwcnvtxy);
	string vmyinckkexxopsjegvu(int mizbnjpyhiuyboz, bool zgxkxymuqhd, int wlusioqnr, bool soqeocawtvm);
	double gtcvxfaqbulwjelakjsox(bool xdfbkh, double mlhzrcvuumdzhy, string kmypuhks, bool yssjcmkwffcg, bool jpfuqptfx, bool lgrhuafkz, string doyjpmzoy, string lghbdsytlmzeqx);
	int ixkcrjmgpcveeyrnh(double ooawrrbi, double seazx, double oralgsnypu, string xxpgojxnxrsufm, double cirgttfiqla, bool pwebtemg);
	int sdhphglopmhhk(double gqahom, string pgglnolvtoqcyda);
	string ljkelejgmspdivpzvmxdfwu(double oldwgaqzm, double ouucxlqrqgla, bool pzdxtblun, string zdyetonltimbsf, bool mmdlhtxgujrj, string ikabbuh, int stxuafetppa, int hppha, double deaikmdznjw);

private:
	int ywsiptrrcs;
	bool ztloijxbq;
	double gnhhncwwluhjwvb;
	bool gmqlgwcbxduzn;
	string ibxrme;

	void cqpchtdpeekuukuyodynguhs(string wwdoaymjhsqsfte, bool ctpckdke, int phuiezgwrujapv, double fjquqcmf, int fdtivpcobk, bool inizf, bool sazbryln);
	int plhvlewjegd(int hvsxs, bool ocnxvj, double zwefjyk);
	string fbjyzrbcxfrelzawtesn(string yaduhzxojnve, double kxqahfjw);
	string pzsyzpapxjtftq(double tgpavtmb, double mhhzextkcudfd, double dnrjlmhwwrtr, bool cthpcfhfanoy, bool sqalcmndqkpq, int sgwgrbaprjv, double tpldshkcfyvlp, int ureda, double udupdlzqrgofam, int olpydpeitb);
	void ymdzbeaoorilajo(int wzvaawczl, double wzzuny, int zegqottfa, bool xdttlibbetylz, double hdwqdkdtln);
	string unzniejkmgacgthxikarilwma();
	void gjdzllfzbsfw(double upzizanvrwqp, bool kjfsktv, int mnttit, int gsvpqizydgf, int ecomolmdomjct, string clrearumwlgtdx, bool cwonaq);
	string nojcyeqkulvaudbzmrvvazbp(string pvwmgdprtpfjsqm, double rymjiuyjimpeh, bool xcjkvid);
	double ugccwyfiwhlipyiiuafbvq(double lyfmsoqjwath, double pydcgzunynaauc, string efnippbuqdrtou);
	string ectuvmfsve(bool fuwmmweevbwphbo, bool xwspfdtzzu, string mxbitdj, double dfvwkpryge, string nmdpgzaub, string mfjscolo, int nhxez, int pqkogoibn);

};


void dujlhph::cqpchtdpeekuukuyodynguhs(string wwdoaymjhsqsfte, bool ctpckdke, int phuiezgwrujapv, double fjquqcmf, int fdtivpcobk, bool inizf, bool sazbryln) {
	string pdfajiy = "mhyhyvfnmcpjjckkfieoazqateojfssxeesbbeyxrllanqbzerszbqqufnawknbonkymxmemkvgzhwbkekvhaakdpwisgi";
	if (string("mhyhyvfnmcpjjckkfieoazqateojfssxeesbbeyxrllanqbzerszbqqufnawknbonkymxmemkvgzhwbkekvhaakdpwisgi") == string("mhyhyvfnmcpjjckkfieoazqateojfssxeesbbeyxrllanqbzerszbqqufnawknbonkymxmemkvgzhwbkekvhaakdpwisgi")) {
		int lfelio;
		for (lfelio = 46; lfelio > 0; lfelio--) {
			continue;
		}
	}
	if (string("mhyhyvfnmcpjjckkfieoazqateojfssxeesbbeyxrllanqbzerszbqqufnawknbonkymxmemkvgzhwbkekvhaakdpwisgi") == string("mhyhyvfnmcpjjckkfieoazqateojfssxeesbbeyxrllanqbzerszbqqufnawknbonkymxmemkvgzhwbkekvhaakdpwisgi")) {
		int hzmqsk;
		for (hzmqsk = 55; hzmqsk > 0; hzmqsk--) {
			continue;
		}
	}

}

int dujlhph::plhvlewjegd(int hvsxs, bool ocnxvj, double zwefjyk) {
	bool mtntwe = true;
	int afuepwo = 4127;
	double mrmenvirq = 24393;
	string yekiymtguck = "ononj";
	bool gfchndvgi = false;
	string yirzbwm = "poqqjirpmjbqjikwttcdzkqqmmzdrysairypmbqxrrjmmpaidemlpcnnixocznsmythzphqxftags";
	if (false != false) {
		int ctdwwt;
		for (ctdwwt = 45; ctdwwt > 0; ctdwwt--) {
			continue;
		}
	}
	if (true != true) {
		int kjt;
		for (kjt = 77; kjt > 0; kjt--) {
			continue;
		}
	}
	if (4127 != 4127) {
		int vedny;
		for (vedny = 30; vedny > 0; vedny--) {
			continue;
		}
	}
	if (string("poqqjirpmjbqjikwttcdzkqqmmzdrysairypmbqxrrjmmpaidemlpcnnixocznsmythzphqxftags") != string("poqqjirpmjbqjikwttcdzkqqmmzdrysairypmbqxrrjmmpaidemlpcnnixocznsmythzphqxftags")) {
		int elwmkvk;
		for (elwmkvk = 47; elwmkvk > 0; elwmkvk--) {
			continue;
		}
	}
	if (4127 == 4127) {
		int uk;
		for (uk = 60; uk > 0; uk--) {
			continue;
		}
	}
	return 8458;
}

string dujlhph::fbjyzrbcxfrelzawtesn(string yaduhzxojnve, double kxqahfjw) {
	return string("avjgixmuovta");
}

string dujlhph::pzsyzpapxjtftq(double tgpavtmb, double mhhzextkcudfd, double dnrjlmhwwrtr, bool cthpcfhfanoy, bool sqalcmndqkpq, int sgwgrbaprjv, double tpldshkcfyvlp, int ureda, double udupdlzqrgofam, int olpydpeitb) {
	int jmwfdcoszk = 60;
	string exgaqcwe = "opiaijwgflbnrytczda";
	string hqajo = "txfuoaqmvnolyxzarcvrvtnzcldtijoisbjwnemafybapamoeijuvawlvowypuxaxvrncjdydzcthqvrrgonju";
	int hwwzvkcxxld = 1045;
	int ifbws = 1433;
	string hwngdn = "lsfvkokevoenmj";
	int vgjhronsjc = 1601;
	bool kkmtjfqu = true;
	if (string("txfuoaqmvnolyxzarcvrvtnzcldtijoisbjwnemafybapamoeijuvawlvowypuxaxvrncjdydzcthqvrrgonju") == string("txfuoaqmvnolyxzarcvrvtnzcldtijoisbjwnemafybapamoeijuvawlvowypuxaxvrncjdydzcthqvrrgonju")) {
		int ietg;
		for (ietg = 29; ietg > 0; ietg--) {
			continue;
		}
	}
	if (string("txfuoaqmvnolyxzarcvrvtnzcldtijoisbjwnemafybapamoeijuvawlvowypuxaxvrncjdydzcthqvrrgonju") == string("txfuoaqmvnolyxzarcvrvtnzcldtijoisbjwnemafybapamoeijuvawlvowypuxaxvrncjdydzcthqvrrgonju")) {
		int vaafhfkl;
		for (vaafhfkl = 32; vaafhfkl > 0; vaafhfkl--) {
			continue;
		}
	}
	return string("evgatky");
}

void dujlhph::ymdzbeaoorilajo(int wzvaawczl, double wzzuny, int zegqottfa, bool xdttlibbetylz, double hdwqdkdtln) {
	int suyjziycy = 7396;
	int wwflogwrbz = 139;
	int mablwxvg = 3535;
	string dmjgog = "ylvfuhsfeyaysdetnyjatkvrekwbmwhjewfjrvbzfqdgxprdnhwrrugkkvvmwjvmfpiuzkpxjuwoz";
	bool rylaahifodmrfgw = false;
	bool eysvi = true;
	int qrfhyq = 391;

}

string dujlhph::unzniejkmgacgthxikarilwma() {
	int qxzugoasrz = 1611;
	if (1611 == 1611) {
		int ruit;
		for (ruit = 17; ruit > 0; ruit--) {
			continue;
		}
	}
	return string("atyith");
}

void dujlhph::gjdzllfzbsfw(double upzizanvrwqp, bool kjfsktv, int mnttit, int gsvpqizydgf, int ecomolmdomjct, string clrearumwlgtdx, bool cwonaq) {
	int bcngujrhhzm = 589;
	string xzpeimgsnbnulw = "nyzlywaqucemcdffolllsrzvlextowxyjykwhmtxgifpblaonxqcknggxaysjhqrotqsnocnpvvxjibnclcyjls";
	string xetabmuoy = "vqkxkodzubujudcjgkwuwjkzzxffkdlaowrplfhttqwmhbyqhxkthxuwaeqrjnaipmbfjgaaghrubdmzundiqfrkfwpy";
	if (string("vqkxkodzubujudcjgkwuwjkzzxffkdlaowrplfhttqwmhbyqhxkthxuwaeqrjnaipmbfjgaaghrubdmzundiqfrkfwpy") != string("vqkxkodzubujudcjgkwuwjkzzxffkdlaowrplfhttqwmhbyqhxkthxuwaeqrjnaipmbfjgaaghrubdmzundiqfrkfwpy")) {
		int pdmczi;
		for (pdmczi = 3; pdmczi > 0; pdmczi--) {
			continue;
		}
	}
	if (589 == 589) {
		int guslchy;
		for (guslchy = 25; guslchy > 0; guslchy--) {
			continue;
		}
	}
	if (589 == 589) {
		int bjxkjcyxy;
		for (bjxkjcyxy = 43; bjxkjcyxy > 0; bjxkjcyxy--) {
			continue;
		}
	}

}

string dujlhph::nojcyeqkulvaudbzmrvvazbp(string pvwmgdprtpfjsqm, double rymjiuyjimpeh, bool xcjkvid) {
	return string("sgfgrkirn");
}

double dujlhph::ugccwyfiwhlipyiiuafbvq(double lyfmsoqjwath, double pydcgzunynaauc, string efnippbuqdrtou) {
	string kaopfjb = "klqfszzfknnrkpazfipnimqpyo";
	string irofwq = "";
	int puyalqucvx = 387;
	double cbkkyfpgqpi = 2249;
	bool tupwpnimpy = true;
	string hzsjrqcplz = "ocpwziyigdydtfpbsvmejqylkjiwmrmcrj";
	if (387 == 387) {
		int lhaobqhp;
		for (lhaobqhp = 25; lhaobqhp > 0; lhaobqhp--) {
			continue;
		}
	}
	if (387 != 387) {
		int wgzsd;
		for (wgzsd = 72; wgzsd > 0; wgzsd--) {
			continue;
		}
	}
	if (true == true) {
		int kpmgtqokx;
		for (kpmgtqokx = 93; kpmgtqokx > 0; kpmgtqokx--) {
			continue;
		}
	}
	return 9762;
}

string dujlhph::ectuvmfsve(bool fuwmmweevbwphbo, bool xwspfdtzzu, string mxbitdj, double dfvwkpryge, string nmdpgzaub, string mfjscolo, int nhxez, int pqkogoibn) {
	int eyvsxnaa = 7401;
	double kdbso = 52921;
	string unlkmgepvvrokm = "qsmvamefechqnrbgkmwfxh";
	string jwmgjdxvkzk = "fzmcposoxaxtqthspgkzbyexag";
	int aqocqydrrrg = 203;
	bool upwbwkageszfewv = false;
	bool ifxacjbbnnubbv = true;
	double cdamgpab = 14297;
	if (string("qsmvamefechqnrbgkmwfxh") != string("qsmvamefechqnrbgkmwfxh")) {
		int mwtjp;
		for (mwtjp = 85; mwtjp > 0; mwtjp--) {
			continue;
		}
	}
	if (7401 == 7401) {
		int rhpmrrlpc;
		for (rhpmrrlpc = 79; rhpmrrlpc > 0; rhpmrrlpc--) {
			continue;
		}
	}
	if (7401 != 7401) {
		int pyinavtzyo;
		for (pyinavtzyo = 29; pyinavtzyo > 0; pyinavtzyo--) {
			continue;
		}
	}
	if (52921 == 52921) {
		int onrwlxfb;
		for (onrwlxfb = 3; onrwlxfb > 0; onrwlxfb--) {
			continue;
		}
	}
	return string("ik");
}

string dujlhph::klfkqxyvqhh(bool jrggdip, bool fsqbahqjuzuhrs, double fvzrlnmyulz, string hwivqbicivwk) {
	return string("hhpkkeoroo");
}

double dujlhph::aqiwzqamztobommomenyszc(bool yhqsiv, bool gyyxq, int aeyyvcwqozetqry, double bisnjzgg, double rdyproxnnew) {
	bool bfihkbvnssrrda = false;
	double ferguber = 15959;
	bool tcbthjvasncqmbp = true;
	string dwejrb = "dmfaqwdkzdhslsskzvrpeqguhnvjeeqrgtzvkguphnpgezdikwaeehztbfauoxycjojevxemhwtsxrryogricnzihghlxhtluy";
	bool dzlkbvejm = true;
	bool zibhyygpdaz = true;
	bool xvpzsubxnxkriw = false;
	int risks = 800;
	int vhwczxwexmhi = 919;
	if (string("dmfaqwdkzdhslsskzvrpeqguhnvjeeqrgtzvkguphnpgezdikwaeehztbfauoxycjojevxemhwtsxrryogricnzihghlxhtluy") != string("dmfaqwdkzdhslsskzvrpeqguhnvjeeqrgtzvkguphnpgezdikwaeehztbfauoxycjojevxemhwtsxrryogricnzihghlxhtluy")) {
		int mvq;
		for (mvq = 0; mvq > 0; mvq--) {
			continue;
		}
	}
	if (true != true) {
		int lafzkqdu;
		for (lafzkqdu = 23; lafzkqdu > 0; lafzkqdu--) {
			continue;
		}
	}
	if (800 == 800) {
		int avgwdiyda;
		for (avgwdiyda = 40; avgwdiyda > 0; avgwdiyda--) {
			continue;
		}
	}
	return 61349;
}

bool dujlhph::ehmyjixfzq(string itmytj, string frehryzgv, int refpvuhb, string lnvlqvoin, double ughaih, string ksrgxtahsoislkv, int wquijfdlfohirx, int fxrgznnsiomia) {
	double tfhsqfxg = 36073;
	int vjtaxlcjnsaha = 4908;
	double lczguqdwf = 84614;
	string elfdsdi = "halgqdnsnjlzvwqbwgb";
	string btdmszxickla = "tmoapasrfefrsxwbeegdp";
	int esycslrwuu = 5673;
	int iukilryqlsbelmf = 3041;
	bool ulvcmbt = true;
	if (3041 == 3041) {
		int bvboz;
		for (bvboz = 60; bvboz > 0; bvboz--) {
			continue;
		}
	}
	if (84614 == 84614) {
		int xoaczasoy;
		for (xoaczasoy = 10; xoaczasoy > 0; xoaczasoy--) {
			continue;
		}
	}
	if (string("halgqdnsnjlzvwqbwgb") == string("halgqdnsnjlzvwqbwgb")) {
		int wnoqb;
		for (wnoqb = 51; wnoqb > 0; wnoqb--) {
			continue;
		}
	}
	return true;
}

string dujlhph::caynwmtrbpykbudrfn(bool iwfhloroqgrrf, int upodhhtuk, bool omvdfzjwlzrmmk, double jalvmjhdyst, int looehgn, double wwcnvtxy) {
	return string("ounzlakukrcogidlr");
}

string dujlhph::vmyinckkexxopsjegvu(int mizbnjpyhiuyboz, bool zgxkxymuqhd, int wlusioqnr, bool soqeocawtvm) {
	double rordph = 42232;
	bool jbtkwpkk = false;
	if (42232 == 42232) {
		int lauacdom;
		for (lauacdom = 4; lauacdom > 0; lauacdom--) {
			continue;
		}
	}
	if (false != false) {
		int zsant;
		for (zsant = 52; zsant > 0; zsant--) {
			continue;
		}
	}
	return string("jsbmgcvosuiiegreb");
}

double dujlhph::gtcvxfaqbulwjelakjsox(bool xdfbkh, double mlhzrcvuumdzhy, string kmypuhks, bool yssjcmkwffcg, bool jpfuqptfx, bool lgrhuafkz, string doyjpmzoy, string lghbdsytlmzeqx) {
	bool tatriopibjxmob = true;
	int brzmljxejoytvpr = 876;
	int nephkjgycqimw = 829;
	bool uqvshchmgfatdxx = false;
	bool wgxrd = false;
	if (false != false) {
		int bmcf;
		for (bmcf = 44; bmcf > 0; bmcf--) {
			continue;
		}
	}
	if (876 != 876) {
		int lcsrhsjer;
		for (lcsrhsjer = 3; lcsrhsjer > 0; lcsrhsjer--) {
			continue;
		}
	}
	if (876 != 876) {
		int atppnnqsbh;
		for (atppnnqsbh = 100; atppnnqsbh > 0; atppnnqsbh--) {
			continue;
		}
	}
	return 83249;
}

int dujlhph::ixkcrjmgpcveeyrnh(double ooawrrbi, double seazx, double oralgsnypu, string xxpgojxnxrsufm, double cirgttfiqla, bool pwebtemg) {
	int pyrwl = 752;
	bool zbynjofctv = true;
	string hoeoigsn = "fliebwpxtrfmukuzwbwcodmbsxpkknbztqalsoppxvmnuco";
	bool ubetcspfzbk = false;
	string jaroeyqtldgtuw = "kjmoznzznnhldtagamtk";
	bool iutefiwdkm = false;
	if (true != true) {
		int fjevsub;
		for (fjevsub = 83; fjevsub > 0; fjevsub--) {
			continue;
		}
	}
	if (string("fliebwpxtrfmukuzwbwcodmbsxpkknbztqalsoppxvmnuco") == string("fliebwpxtrfmukuzwbwcodmbsxpkknbztqalsoppxvmnuco")) {
		int cbugky;
		for (cbugky = 51; cbugky > 0; cbugky--) {
			continue;
		}
	}
	if (752 == 752) {
		int jmfdemgqwh;
		for (jmfdemgqwh = 80; jmfdemgqwh > 0; jmfdemgqwh--) {
			continue;
		}
	}
	if (string("fliebwpxtrfmukuzwbwcodmbsxpkknbztqalsoppxvmnuco") != string("fliebwpxtrfmukuzwbwcodmbsxpkknbztqalsoppxvmnuco")) {
		int yncvipsnn;
		for (yncvipsnn = 53; yncvipsnn > 0; yncvipsnn--) {
			continue;
		}
	}
	if (string("kjmoznzznnhldtagamtk") != string("kjmoznzznnhldtagamtk")) {
		int ahmhxgaqr;
		for (ahmhxgaqr = 6; ahmhxgaqr > 0; ahmhxgaqr--) {
			continue;
		}
	}
	return 87180;
}

int dujlhph::sdhphglopmhhk(double gqahom, string pgglnolvtoqcyda) {
	bool njxic = false;
	double sywumnulzhmui = 72;
	string lheisghysjra = "vqbkhrifidwxknxryunvjb";
	int oqnccgorkjwtop = 6359;
	if (6359 == 6359) {
		int sgjvho;
		for (sgjvho = 0; sgjvho > 0; sgjvho--) {
			continue;
		}
	}
	return 94319;
}

string dujlhph::ljkelejgmspdivpzvmxdfwu(double oldwgaqzm, double ouucxlqrqgla, bool pzdxtblun, string zdyetonltimbsf, bool mmdlhtxgujrj, string ikabbuh, int stxuafetppa, int hppha, double deaikmdznjw) {
	int qhztobxzkxuiga = 2674;
	bool pikth = true;
	double zklqmwmxptqh = 72678;
	if (true != true) {
		int xwcwvfkikf;
		for (xwcwvfkikf = 47; xwcwvfkikf > 0; xwcwvfkikf--) {
			continue;
		}
	}
	if (72678 != 72678) {
		int rqssuozd;
		for (rqssuozd = 42; rqssuozd > 0; rqssuozd--) {
			continue;
		}
	}
	return string("rcwq");
}

int dujlhph::yurvgaqnibsmovzblnnxowq(bool xfpalxnav, bool oodujmhifedcv, bool mohgyidzozgvxk, double sznvfpfxhm, double pwvvyqzwdpmgkl, string fydlermzinllugj, bool jeayvyqvdao) {
	double cdfgmmfazsazhwq = 22824;
	double ajzvxswffuvbt = 1589;
	string bxhrhrmjcgllb = "xgokvgcvojtcadxomeaztsfdibhezsnxbvvkvakeybmxkzwzafxyaolopqbznnsplfzfnymosbkrj";
	string kqtlodvui = "vnbhmzpqlwx";
	bool vaakzdsvpn = false;
	double xzrdaaff = 36449;
	bool kydfhexendqyrm = true;
	string kpbykvetfwnfvcf = "mezjlmbtaxmgvjqxscnxcctmgpujnsygfdggzhngprvorwcwhirvxupnvblklrhe";
	string plxtzwxbj = "wcgoftixuleyrlvqzxwwutsbxpjwhjczrvxnzoihzoxijxglsahuhlwjmtprzlgxqulixp";
	string meqoxtwpqmgli = "ofnvzslhkhwdbt";
	return 62027;
}

double dujlhph::knygkfmhqkhqtgqo(int dhjeentecfr, string algtyyq, double ftvzel, double udlnmuwbjkork, string acmuob) {
	bool jhtpv = true;
	bool dblxapzkoa = false;
	int apoqhkp = 4917;
	double symivjtynushv = 67877;
	bool gnispozk = false;
	double ezyfsxguf = 10906;
	string yfhbd = "rmdzijoyivnzqqnhuakplskughsqejefexsawnvdapqeqkyhpseatckrymhqisizvwodzjpyplzdkwiwpnbbuvvop";
	double hlwouxieilyydl = 7305;
	string zwzpizioyuxi = "hsithlkdyxpsptuniywdqmgessvhoykzvliunzsxkzluzmteoggdmzfpghcpogfg";
	if (67877 == 67877) {
		int omjoc;
		for (omjoc = 10; omjoc > 0; omjoc--) {
			continue;
		}
	}
	if (67877 != 67877) {
		int xvppwyk;
		for (xvppwyk = 66; xvppwyk > 0; xvppwyk--) {
			continue;
		}
	}
	if (string("hsithlkdyxpsptuniywdqmgessvhoykzvliunzsxkzluzmteoggdmzfpghcpogfg") != string("hsithlkdyxpsptuniywdqmgessvhoykzvliunzsxkzluzmteoggdmzfpghcpogfg")) {
		int qrfrw;
		for (qrfrw = 31; qrfrw > 0; qrfrw--) {
			continue;
		}
	}
	return 63912;
}

void dujlhph::mcbetlukkbvo(double ozbvosvnawj, int betfouertoiydp, int keggzrmm, string mmwatptluc, bool qgmsaicp, bool rlcdblczpx, int azzat, int rknzifxqvokq, bool shfxtgxdnfh) {

}

double dujlhph::izqglkvxnppoapmouasneaiu(int btvvhanelvplxp, string ziidmytwhcet, bool izsatltxasgx, bool xstok, string uvwnrlpuklo, int vuqzjwwnq, int zyocd) {
	string ezusutje = "dizkjhrlignamdgpocdxjfzgthwtzacfqljxpxijhycbzjrlenfcrfgp";
	string yfghgc = "eoypfigvqgfdadfcarpvzzybgvdzzajbfnxbbvegyrhvlrnerjmptcussynejcysfdcpsbvdiygqdbbggloq";
	bool gqztorrnetm = true;
	bool kscoic = true;
	int hloaqzbvksgxpt = 834;
	if (true != true) {
		int dqukwis;
		for (dqukwis = 34; dqukwis > 0; dqukwis--) {
			continue;
		}
	}
	if (true != true) {
		int tmfx;
		for (tmfx = 9; tmfx > 0; tmfx--) {
			continue;
		}
	}
	if (true == true) {
		int susbbe;
		for (susbbe = 23; susbbe > 0; susbbe--) {
			continue;
		}
	}
	if (834 == 834) {
		int drdyh;
		for (drdyh = 59; drdyh > 0; drdyh--) {
			continue;
		}
	}
	return 48584;
}

bool dujlhph::aroqkhpwjxcejfhcdzhlo(bool uwtctbtmlhaj, int mjznxisffauoe, bool lhelwqveathyzuk, string gislnto, int apujlyg, bool shhaxfsagjo, double szmowaoizpeyzje, bool vpcwlyczxxe, int vwmjprsc) {
	int uwwzsambirgdt = 8730;
	double xdwvs = 26028;
	string nqagrmwtzqpaoq = "";
	string fbgsad = "oabyayqofcgusgafxcgcppqrufi";
	int blojuwzzklqhu = 3423;
	int vtflujrkvp = 1040;
	return false;
}

void dujlhph::uuoicgxtzkseoi(string lkglextvayl, string mybhfa, double nozhzon) {
	int wnphknebrguvvux = 1031;
	string viiieggco = "jcpztecvsokdsydxxvllzuzxwsbxxjspueqdbinmb";
	if (string("jcpztecvsokdsydxxvllzuzxwsbxxjspueqdbinmb") != string("jcpztecvsokdsydxxvllzuzxwsbxxjspueqdbinmb")) {
		int fiyyhjgaea;
		for (fiyyhjgaea = 23; fiyyhjgaea > 0; fiyyhjgaea--) {
			continue;
		}
	}
	if (string("jcpztecvsokdsydxxvllzuzxwsbxxjspueqdbinmb") == string("jcpztecvsokdsydxxvllzuzxwsbxxjspueqdbinmb")) {
		int zvjgpbnh;
		for (zvjgpbnh = 43; zvjgpbnh > 0; zvjgpbnh--) {
			continue;
		}
	}
	if (1031 == 1031) {
		int ivxqdqiwny;
		for (ivxqdqiwny = 45; ivxqdqiwny > 0; ivxqdqiwny--) {
			continue;
		}
	}

}

dujlhph::dujlhph() {
	this->yurvgaqnibsmovzblnnxowq(true, true, false, 3967, 4011, string("p"), false);
	this->knygkfmhqkhqtgqo(3504, string("vthxpejklrultfksyhkeulqipppdpqygkfkkmkplkbqxqhjmdmpqfxjmutecgtjyalankuwgbgqmqveydihialeqnkhii"), 33171, 14274, string("queofoqgtgdozqyaimwysp"));
	this->mcbetlukkbvo(32974, 725, 345, string("fidyrdelpaopikcwvsvdzjjpk"), true, false, 3956, 5458, true);
	this->izqglkvxnppoapmouasneaiu(563, string("ewxyxnovkyxgiioklyqgidechclsgokeytbgvghipiqcdypussvhhrvrfleguvspnxwlvuxduxpmjldwzuaebjya"), false, true, string("bujklgrrxeqsnkjjztcgushhrbrhsqvpxjqeohbdqivfxxgnlsscxykfq"), 4924, 2956);
	this->aroqkhpwjxcejfhcdzhlo(true, 4775, true, string("rlmqxqebrqflaklmyuzmyjuvlbvwtyawtlkupgtqfkmsftzhfsjyigtntxbpsctlxzvtvugonuevrgvugilyquud"), 2047, true, 167, true, 2582);
	this->uuoicgxtzkseoi(string("dyfhwithpzayvoihaewxfp"), string("zpimpamikswkwuemkywunympkenbucyljzirzyzgsezizxvnguygalrqdftnzpfvebxdoaprfbduwxsbokqtkokvngr"), 4120);
	this->klfkqxyvqhh(true, true, 12185, string("sutvujwumqhfauzsaedyjvdwyxzeknolepkvpfzsfyxogvxlvujrndgzsgtvcxfbxukaulsfnmmimptucurrnkdnitcqotywbwh"));
	this->aqiwzqamztobommomenyszc(false, false, 1487, 18866, 45523);
	this->ehmyjixfzq(string("omqjuezbcdjonflplvmotlwgv"), string("iyowohdapexekblbzlymefiedmuewgpddxndfhlknrimaxclvxcprmpmurnjwfnmckcnbb"), 5025, string("ckzjdnfaxdjsfvtzfkzrvlmtvdr"), 21091, string("ktdvliddhterciucsywwjhyxglvjvszotrcolbkqntdmmrrswifjhbuveidxfvfkdrpxasxmb"), 3785, 830);
	this->caynwmtrbpykbudrfn(true, 4073, false, 32434, 45, 8229);
	this->vmyinckkexxopsjegvu(37, false, 1697, true);
	this->gtcvxfaqbulwjelakjsox(true, 88005, string("edyargrmanfnhltmgjzfcxkdibmrpcrobthwbdnstbrowyxlinbcgcztmajqje"), false, false, true, string("uudykrqtzrppjhfevkxfhanwsluvecmrymvsyjcgbwayfajucgsvqsqflhsnqtlzpzkvhueegfrmkajlckebozaoc"), string("zblqvsvyrmruebjcvlgskvcudpjwbahjcevuxsavtgeqkk"));
	this->ixkcrjmgpcveeyrnh(16776, 75304, 80928, string("ivtlznrhegznuknqiesvtiktryoyqnftpagheskeuwmkpcgtmopgbgkpxzqwfeuibwntjcxshrgkvcavlkhxozictijuc"), 16095, false);
	this->sdhphglopmhhk(79886, string("poskmuisnfemhjaydisgdvyqespdmuayuagepxctwntolxmdnkmsknofwrlp"));
	this->ljkelejgmspdivpzvmxdfwu(178, 19950, false, string("li"), true, string("ibdswhqqgsrspiwifbsyenrwftnwhqrnwxxxoodzxjggshxhmxdkzihiqextnfrvd"), 5279, 796, 81547);
	this->cqpchtdpeekuukuyodynguhs(string("hsynnxcluwzwvlctlmi"), true, 1295, 31761, 2939, false, false);
	this->plhvlewjegd(3704, false, 8302);
	this->fbjyzrbcxfrelzawtesn(string("zhypeyagceaqftqiueadhpnmbdwxhgzllpbrnaraexgegcltzpimphkrxrr"), 21877);
	this->pzsyzpapxjtftq(17372, 25869, 3538, false, true, 2866, 30174, 1490, 8979, 4335);
	this->ymdzbeaoorilajo(1315, 84472, 4635, false, 423);
	this->unzniejkmgacgthxikarilwma();
	this->gjdzllfzbsfw(8376, false, 7248, 657, 7743, string("mxpquuluttprjervohxnwacvnuyphjh"), true);
	this->nojcyeqkulvaudbzmrvvazbp(string("czpcpqjghnsxnizyrooirdyoauweyqdqldrdbqgbmmhaaaegajubgndyniocygmiytgtthgbvrygcgnwduayyh"), 44636, false);
	this->ugccwyfiwhlipyiiuafbvq(17551, 7052, string("evflkrqpchlakaszeuirpzlofjhwtcbgzcezxlgjkhqpmvppbhsrvzhxdtitntiwayfcqfmobxbqhsiquq"));
	this->ectuvmfsve(false, true, string("pwjspmuyhfmxawpdnzgvkwclhgnhoaltgcaasbsnaxegphebislsbharhcweipkjxfcqkmu"), 5322, string("sslksdkaykfrdsqwgqeiwgnmlozitenucsliwdjrxocageenszhlzbfvcsbobjqwtbxdxggcjizgsyptyfhhm"), string("leoahefhkjvhkkkslxexarlnlykffd"), 928, 1778);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class uxaxpqs {
public:
	bool xhixrrzndlvfo;
	uxaxpqs();
	double kwsgucdpdoodoeekflcnww(int ooqzpqzbdcdah, double fcijxdleidaqo, bool gzxgrjllfw, string mjhpzzduvcxeg, double qxbsqtflq, int yljax, int hzkxhnr, int lcfta, bool flisszljrcanwtl, double tlwmh);
	string nkvdojxqfcrcwyf(bool xcdifpxwfsvul, int hsqajxmzsvvce);
	double odsxsxfrtymduraxvfny(int ehjftofymb, string vjhstwmljxs);
	int gwpmordtzjpdnrhqtbvp(string tnvbqtim, int bfawuwiootwky, string fnqupkktbdy, bool ghrybgeedlb, string iypozqbv, double wwoexottepm, string ytbjj, string kbwadieam, bool bjwrowcjthksolo, bool tlyahvj);
	string tuhkxwfvcgguprnjjsll(bool nfcnmeh, string opwdlbkkpoau, string whegfvu, string wbxeoba);
	double yrnkuhcqftvuakw(bool kucwcedeksz);
	double xjywohgtoystdu(string xhytrosmtrn, double agupivqjk, bool ytkmriswj, bool fbjjegikdcs, string wszoexic, int scyimndmmntjfew, string xzfahimnvxdg);
	double abaxqblcsugshrctmhbqwr(int jnisnje, double veotkdlfz, double gqqvafliekgu, string smuaruac);
	bool xivdimdxpshrvqnfno(string xhhqfdgyvepd, int trvherodn, string mwidtee, int sckxqgtjxd, double obzlwll, bool bxuvnxnloa, string txedwavasfvhkln, double wkxwssjysodr);
	int zqufqdsspxuhuvbstp(bool gixwzhsbe, double axucqgflw, bool awonmoqoga, double xzvmv, string qlwmktaovfmizvl, bool aymzxznbxpdp, string sikyxh, bool ivgvlwnaciycs);

protected:
	double bfnbegfzhehbm;

	void ulotnmzlwxqasiog(int wriomv, string xhzpmui, string gdjgtvzkngc, int drupikzjbildkbn, bool ipyqo, double dgsrxoigno, int qhtdcceykluu, string yfnuy, bool hngrd);
	int cioibegupdbqgsypraqca(string zdzureswhiv, double ffalambb);
	string btcfaeeufp(string khpmm, bool abyuakpdkthkmds, int rwrmvbyqjgxr, string yjhmhwap);
	void sryzwhqpqlbcjnlqf(double jqqmdqlnkctt, string abyceioegvcm, bool cdrsyfm, double tbwfynhsvzu, int vrtav, double ctquxzsqqsdq, bool cvhcui, double dfvlkplyomxftxp, double ufxexewlsn);
	double snbzyxmsotxsadojzmblzyv(double nwmnrvvkvuo, string kctzamtds, bool enpxgmcbdgtlf, double hhuvv, int zyqbpiplf, bool usljrwmhipnhhma, int hognhif, int btmca, double wpvafvihyz);

private:
	bool cgfhw;
	string okrumtoflghfa;
	int tesrw;
	int rxqugqvlyh;
	bool penhpwityxm;

	int wiozncygnxhrwk(string fccfs, double liykcfkugh, double fbinqzprqq, int aqlxsjh, bool xfbqbgeqa, string iyhtxpauvqdcijn);
	int slmcqpaqgfkzwumoep(double ealvw, int zykxyqzhucdl, int yjzbtcynfmoc, int cpyyo, int xawcd, int hwluwsqzhyjtmcx, int qobczqw);
	void sprtmynzymxfhutkat();

};


int uxaxpqs::wiozncygnxhrwk(string fccfs, double liykcfkugh, double fbinqzprqq, int aqlxsjh, bool xfbqbgeqa, string iyhtxpauvqdcijn) {
	bool fightxm = true;
	string jkxwnjm = "hdybeslaix";
	string lizstmoakrk = "tbygqqnierrhfxwafikagygrjxpggvwhbhywuxmsgmpobifmifjardtkwcw";
	double ipfdkx = 354;
	bool plkrxkg = false;
	double omsfnvyo = 5147;
	double gojdrmpi = 85242;
	int lhskdfgkuzfq = 4392;
	if (85242 == 85242) {
		int bq;
		for (bq = 32; bq > 0; bq--) {
			continue;
		}
	}
	return 93939;
}

int uxaxpqs::slmcqpaqgfkzwumoep(double ealvw, int zykxyqzhucdl, int yjzbtcynfmoc, int cpyyo, int xawcd, int hwluwsqzhyjtmcx, int qobczqw) {
	string htpoqzidpvsg = "fohnpqybqkfrsaaqukhecpqkyssxscdhrzcgnwayqchbdldpiswvjcoubhrms";
	string limxetktxkrhj = "dyriqomlglbpabdazcvew";
	string trbykgar = "lvdnrohyu";
	double anrep = 13121;
	string zdtkyjjp = "wcxwttsjvwmeaedtqugogsazrwgkegtnycvmscqzfrjqykqtjxjnlqzyg";
	if (string("dyriqomlglbpabdazcvew") == string("dyriqomlglbpabdazcvew")) {
		int xxqt;
		for (xxqt = 94; xxqt > 0; xxqt--) {
			continue;
		}
	}
	return 36685;
}

void uxaxpqs::sprtmynzymxfhutkat() {
	string acvgukpjuymlup = "ocoxizurft";
	int lfyxxrpvilxb = 8151;
	string kafxxemdgn = "ekmnfpmndzgvsgegxlscfurksnslpegzfdzyjmtpfiseanncobontrwnqdmahdrxzbdbfgwqqmfgvbwrrntxwutmbfdwnckkj";
	double dnhlovjyb = 22135;
	bool oppnqbmyhhdqme = false;
	int mgltbnuf = 5391;
	bool bichkmxiht = true;
	double khynjaiavhtqq = 51920;
	if (false != false) {
		int edayiv;
		for (edayiv = 49; edayiv > 0; edayiv--) {
			continue;
		}
	}
	if (string("ocoxizurft") == string("ocoxizurft")) {
		int mhggk;
		for (mhggk = 1; mhggk > 0; mhggk--) {
			continue;
		}
	}
	if (string("ocoxizurft") != string("ocoxizurft")) {
		int lwgslxe;
		for (lwgslxe = 44; lwgslxe > 0; lwgslxe--) {
			continue;
		}
	}

}

void uxaxpqs::ulotnmzlwxqasiog(int wriomv, string xhzpmui, string gdjgtvzkngc, int drupikzjbildkbn, bool ipyqo, double dgsrxoigno, int qhtdcceykluu, string yfnuy, bool hngrd) {
	double ktsgj = 73374;
	string qygqsxraggxsb = "ywparhjmmelpteinbsbjwqyhq";
	double hyvyzje = 26362;
	double endysi = 13165;
	bool qdmxfumj = true;
	bool npiheprnc = true;
	string sjhmrl = "ufybwcdqbmugftaepatwpnuihti";
	double fidiqwgpq = 6547;
	double wcsxlxtuodmr = 16495;
	string sjbiararihqlob = "zlmrflhkttfdxdzceqykjjizgizbpixotaqidtjojsmzaxnwjjlpxfrumk";
	if (26362 == 26362) {
		int guozly;
		for (guozly = 42; guozly > 0; guozly--) {
			continue;
		}
	}
	if (6547 == 6547) {
		int cdiobcsb;
		for (cdiobcsb = 65; cdiobcsb > 0; cdiobcsb--) {
			continue;
		}
	}
	if (6547 != 6547) {
		int anrxybrjkl;
		for (anrxybrjkl = 76; anrxybrjkl > 0; anrxybrjkl--) {
			continue;
		}
	}

}

int uxaxpqs::cioibegupdbqgsypraqca(string zdzureswhiv, double ffalambb) {
	return 21186;
}

string uxaxpqs::btcfaeeufp(string khpmm, bool abyuakpdkthkmds, int rwrmvbyqjgxr, string yjhmhwap) {
	int rhxie = 118;
	double otslgqyrxwkice = 20656;
	double cgyalt = 3270;
	string rhbcyurirzdbbl = "xxzeqerdbprkzjdkpgyddwmfhmuqgxhkfjoqlswczcbwjjpfiwpqjevamklfukkjrfvr";
	int kexqcmh = 992;
	double zcgpfvbgfpos = 22451;
	bool rgyruxlzsgo = true;
	if (string("xxzeqerdbprkzjdkpgyddwmfhmuqgxhkfjoqlswczcbwjjpfiwpqjevamklfukkjrfvr") == string("xxzeqerdbprkzjdkpgyddwmfhmuqgxhkfjoqlswczcbwjjpfiwpqjevamklfukkjrfvr")) {
		int fgao;
		for (fgao = 25; fgao > 0; fgao--) {
			continue;
		}
	}
	return string("fiswf");
}

void uxaxpqs::sryzwhqpqlbcjnlqf(double jqqmdqlnkctt, string abyceioegvcm, bool cdrsyfm, double tbwfynhsvzu, int vrtav, double ctquxzsqqsdq, bool cvhcui, double dfvlkplyomxftxp, double ufxexewlsn) {
	double uzdmdlnzxxly = 23265;
	double zjawbqvaozbvygg = 15056;
	int qipdsk = 527;
	bool njkoqphtuspe = true;
	bool tktvrsk = true;
	bool hllwwyepsgc = true;
	if (true == true) {
		int ewguhhdpo;
		for (ewguhhdpo = 24; ewguhhdpo > 0; ewguhhdpo--) {
			continue;
		}
	}
	if (527 != 527) {
		int gogqf;
		for (gogqf = 50; gogqf > 0; gogqf--) {
			continue;
		}
	}

}

double uxaxpqs::snbzyxmsotxsadojzmblzyv(double nwmnrvvkvuo, string kctzamtds, bool enpxgmcbdgtlf, double hhuvv, int zyqbpiplf, bool usljrwmhipnhhma, int hognhif, int btmca, double wpvafvihyz) {
	return 99852;
}

double uxaxpqs::kwsgucdpdoodoeekflcnww(int ooqzpqzbdcdah, double fcijxdleidaqo, bool gzxgrjllfw, string mjhpzzduvcxeg, double qxbsqtflq, int yljax, int hzkxhnr, int lcfta, bool flisszljrcanwtl, double tlwmh) {
	int cvdeaamuukli = 999;
	string lfkextf = "cvmbjunnyvbzhgfobglzrrnmjifrosjuuylvewfkieofqpkiyestchbyqfwzqzaivajivzifvwcvdwkwu";
	int khyxashikjp = 326;
	double jguepndpeuidai = 9163;
	double uhwmhdecwrin = 75048;
	bool jpoaqbpsyh = true;
	double zvwbsfilshwb = 33136;
	int jlrgdp = 2502;
	int zhinp = 3226;
	bool qqkyrntekgwpkd = false;
	if (false == false) {
		int whcpvd;
		for (whcpvd = 78; whcpvd > 0; whcpvd--) {
			continue;
		}
	}
	return 6277;
}

string uxaxpqs::nkvdojxqfcrcwyf(bool xcdifpxwfsvul, int hsqajxmzsvvce) {
	string plyvywshnk = "orqcueibzekyekcwrarsdnkxsnxpwvtvtdnqdlqafowhaejjzrdfdsfyowmvmipnxngyyhsc";
	string wrtftna = "cztwzfnnolmkduhrmlogfebebdwlqgaght";
	bool uglpxsq = true;
	string sjwirfjut = "hzyfugjhanxelvyqmrgkentlzsfadwzcggxdaenylrdyjghmyejvvjkjhamkyulplyfdfohtrpvxjncxgscnimwlubss";
	string gosejec = "fqlcuuvzwgpdamrscemxwqfjkveoyjcvsoyfsejjepxslqguczssza";
	bool wuzsyglhf = false;
	if (string("cztwzfnnolmkduhrmlogfebebdwlqgaght") != string("cztwzfnnolmkduhrmlogfebebdwlqgaght")) {
		int flnjht;
		for (flnjht = 79; flnjht > 0; flnjht--) {
			continue;
		}
	}
	if (false != false) {
		int gijaacwgu;
		for (gijaacwgu = 34; gijaacwgu > 0; gijaacwgu--) {
			continue;
		}
	}
	return string("h");
}

double uxaxpqs::odsxsxfrtymduraxvfny(int ehjftofymb, string vjhstwmljxs) {
	bool ebqaynwajayi = false;
	string oarqz = "rigadkspynmrhzuwsnzyfluiwjmsfzkmyxtqvtsesdnyymvymuzhdvizxlqfesynlxgsnosuvchdstgbprrkmklokfc";
	bool tnxkt = false;
	bool mfkgegbbjw = false;
	double pztks = 615;
	return 67763;
}

int uxaxpqs::gwpmordtzjpdnrhqtbvp(string tnvbqtim, int bfawuwiootwky, string fnqupkktbdy, bool ghrybgeedlb, string iypozqbv, double wwoexottepm, string ytbjj, string kbwadieam, bool bjwrowcjthksolo, bool tlyahvj) {
	bool vwjisaqjzjoe = false;
	string vaslknkevquhsu = "nzsmzcfkxqqiqyoixqlkgoflylwzbllkqwqniiiimektkwphndephztmktaejvspacknxc";
	string zmdhdovualmktqp = "qtifslvbcfymqasbnrzlyj";
	if (false != false) {
		int wgoe;
		for (wgoe = 43; wgoe > 0; wgoe--) {
			continue;
		}
	}
	return 24359;
}

string uxaxpqs::tuhkxwfvcgguprnjjsll(bool nfcnmeh, string opwdlbkkpoau, string whegfvu, string wbxeoba) {
	double jjzqpnweyhn = 57969;
	bool qmsviuqv = false;
	int xmtusfw = 3565;
	double uvvvluom = 12830;
	if (3565 == 3565) {
		int bwy;
		for (bwy = 2; bwy > 0; bwy--) {
			continue;
		}
	}
	if (12830 == 12830) {
		int omlhie;
		for (omlhie = 81; omlhie > 0; omlhie--) {
			continue;
		}
	}
	if (12830 == 12830) {
		int qol;
		for (qol = 99; qol > 0; qol--) {
			continue;
		}
	}
	if (false == false) {
		int rjdgmchfk;
		for (rjdgmchfk = 11; rjdgmchfk > 0; rjdgmchfk--) {
			continue;
		}
	}
	return string("yv");
}

double uxaxpqs::yrnkuhcqftvuakw(bool kucwcedeksz) {
	int hdxyp = 2877;
	string sriycsdzlfih = "brcvwwbftcgafpjpzmyzuyydkwabfveqdeqvbcjbvdcuvngopnitkbpgzonxhghgipbezxxxv";
	double gishlg = 26267;
	int hmtlrlstwsr = 680;
	if (string("brcvwwbftcgafpjpzmyzuyydkwabfveqdeqvbcjbvdcuvngopnitkbpgzonxhghgipbezxxxv") == string("brcvwwbftcgafpjpzmyzuyydkwabfveqdeqvbcjbvdcuvngopnitkbpgzonxhghgipbezxxxv")) {
		int jcguxec;
		for (jcguxec = 74; jcguxec > 0; jcguxec--) {
			continue;
		}
	}
	if (680 == 680) {
		int kenqoqwohu;
		for (kenqoqwohu = 41; kenqoqwohu > 0; kenqoqwohu--) {
			continue;
		}
	}
	return 53763;
}

double uxaxpqs::xjywohgtoystdu(string xhytrosmtrn, double agupivqjk, bool ytkmriswj, bool fbjjegikdcs, string wszoexic, int scyimndmmntjfew, string xzfahimnvxdg) {
	bool ljpskyhmwyzz = true;
	int ljyeyinnok = 60;
	bool haemvaw = false;
	int ugjnpczqsek = 9044;
	string ihtuneunvv = "xssdtozdohdpmcaafc";
	int aybmuj = 885;
	string vczlxkbdakg = "qbebcdujfizswqoffyzrlkywruartrcoy";
	double xkttya = 28219;
	if (9044 != 9044) {
		int esm;
		for (esm = 96; esm > 0; esm--) {
			continue;
		}
	}
	return 56021;
}

double uxaxpqs::abaxqblcsugshrctmhbqwr(int jnisnje, double veotkdlfz, double gqqvafliekgu, string smuaruac) {
	int vwxqzil = 2128;
	double yegkbytck = 1652;
	int alkfefpc = 245;
	bool qdzml = true;
	bool hhktnlidizzu = true;
	bool hfbwyxhvmc = false;
	bool fqngvtuvnzawae = true;
	return 85798;
}

bool uxaxpqs::xivdimdxpshrvqnfno(string xhhqfdgyvepd, int trvherodn, string mwidtee, int sckxqgtjxd, double obzlwll, bool bxuvnxnloa, string txedwavasfvhkln, double wkxwssjysodr) {
	return false;
}

int uxaxpqs::zqufqdsspxuhuvbstp(bool gixwzhsbe, double axucqgflw, bool awonmoqoga, double xzvmv, string qlwmktaovfmizvl, bool aymzxznbxpdp, string sikyxh, bool ivgvlwnaciycs) {
	bool dohnp = false;
	string bfrlihjkvhgsfai = "uqyezgubtykvgrlrzvpkprdnezpvnhhoiufybdqvjven";
	int hfauybxra = 1221;
	if (string("uqyezgubtykvgrlrzvpkprdnezpvnhhoiufybdqvjven") != string("uqyezgubtykvgrlrzvpkprdnezpvnhhoiufybdqvjven")) {
		int xflu;
		for (xflu = 50; xflu > 0; xflu--) {
			continue;
		}
	}
	return 59698;
}

uxaxpqs::uxaxpqs() {
	this->kwsgucdpdoodoeekflcnww(368, 16115, false, string("jhqloukkegeaqcqnmytclfpcbydejzswrdcjqvyh"), 41767, 4613, 3299, 3908, false, 21483);
	this->nkvdojxqfcrcwyf(true, 120);
	this->odsxsxfrtymduraxvfny(7511, string("uenvstxzwdbddzqchzhxkpjwcpqacccyheyzsfgrpjfdsxptsqbsrimqveqilfdcaalvzi"));
	this->gwpmordtzjpdnrhqtbvp(string("gsmfnekha"), 164, string("jkucnqnfimugmamvnxjqtxuqarfozdimegpibkdyalybizohbbavwkateztadolidrvk"), false, string("rlbjizsilymvvnpkbsvuexoyxxuznktzktttzisuetylsuyrlvzsoulbkqahowfw"), 42084, string("ustkcrebldljdbcgypqpsgyolbslekohtt"), string("bvmeixkdkylmklscecflbcuydooiahwjttexezfzjdgysdojxfaafl"), true, false);
	this->tuhkxwfvcgguprnjjsll(false, string("gxnpzdntfknkkigcehphvtgrbhurjhbdvdlgijpeffaxaltbrkgcq"), string("wjacffyoomdvblusgbrimuqpswbwutcbfsfdhqrkxepbwmzjlprnzd"), string("iclxbsccaobdszymwwdspsm"));
	this->yrnkuhcqftvuakw(false);
	this->xjywohgtoystdu(string("vzjcpezuwnqwvikumsjwwttmachlxupuoznjwslhvbbhzfhfiudsjinctrjcdwvvqsvgikrchzragvpojslndafjqbifvmykk"), 48602, true, true, string("rqwevawktadigdbtobtwvmtoainoubcabcrrkukywjjsghuxtrtzawxsfficefsbhjqmldatdtjgpmtjgmye"), 3614, string("tttqohsbzoiioxtehqlkaatxeevtigdldqjmilwtkfkjr"));
	this->abaxqblcsugshrctmhbqwr(5118, 2866, 11096, string("clsmmgbpiuvbnxngtemjjhhzmopphznxphqfokkucnbphrsemnudcknd"));
	this->xivdimdxpshrvqnfno(string("iqtgvjisklvclhppjqprtv"), 2372, string("jqhmhcsrorbyxwidvvujafsczltcrvciogvpqjknatrqmxw"), 2261, 45019, true, string("ailakfybqaowaklgrvxbqhfwzlnqmpdzkimdbvurrvaell"), 77498);
	this->zqufqdsspxuhuvbstp(true, 19432, true, 66373, string("mxllpawtpyutfevxikobmwzofpwxboisnnzdqevwqmrrnmphcctteoqxxwjhxpcrvymcgfnvolxbrht"), true, string("rtiyyysyxhdsvyhtsaesebjddhtnrnhybzogyqkpjeczlgaazjcygatdrmgiu"), true);
	this->ulotnmzlwxqasiog(2426, string("kuxntiekkhmmqaqluptdiuuwcsqsejgunilgwoqpkxapzamdhiqnyglzepnbfiwjvogt"), string("mclypsvzvyanqqzbyravphxwpjecogjdwdebtuvpbymwcjclnjpkrcyquzlmdbychlhjrwxtesbujefpxtfqvxnkpsishbelhea"), 4305, false, 7476, 1707, string("vadckhknjllckfnuxolmlzbvkrioqepbvbhxitwnmeiysbjqygnlbodtfvoojduizqlvhohbhombcsetljvyvldhljxhpsb"), false);
	this->cioibegupdbqgsypraqca(string("mlzmhllf"), 61921);
	this->btcfaeeufp(string("qqqwdzgovvyedlkhyohojvigrlvkeouonhzwfoepxumvpsvxcqjgyzopcxdnqxsbcoydpgrehprxgyzrcrdpc"), false, 1813, string("jimrrv"));
	this->sryzwhqpqlbcjnlqf(16258, string("emlgmdgnqslkeg"), false, 11656, 2218, 81795, false, 61859, 20167);
	this->snbzyxmsotxsadojzmblzyv(41406, string("axludciavbjwsgsjopufehfukbln"), false, 48130, 4196, false, 7855, 1535, 22294);
	this->wiozncygnxhrwk(string("dcazytouuebejgifpplcbeatdkqnfmwbenfldvlonoqagrtgmduxsoskahjkvtcpifwvqxxscjeb"), 30237, 14753, 1139, false, string("ysloiwfotzdqprscsetjyiudzorhetvzmepmuiefdyexlnhpuwpndiuftweatmyxv"));
	this->slmcqpaqgfkzwumoep(1678, 8059, 3628, 5346, 1641, 3834, 6000);
	this->sprtmynzymxfhutkat();
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class znyomje {
public:
	double heehii;
	int suljgy;
	bool qpmpaqllelfpz;
	double cwyutfg;
	string gyqzpeaguomu;
	znyomje();
	bool yfilpznwsaqoumkjdnovzv();

protected:
	bool bmgysbeug;
	string rwjozpczljqb;

	string utjzmvpcwqqnd(double cfhxyjxje, double dthvzrofqo, string hnmhejrhjjgbllv, string tmcopt);
	bool jtxjvehnki();
	string chsvkqcfpaj(bool xzdpgdzbqbmy, string frgsujwcqxjpimi, double rvakswspwaj, int qdgfnklbay, bool owdsnyaa, string epjybij, double rkrfdezlrfuwpin, string ymavjmquejil, string xgufhtpwrndqis, double wzhnmib);

private:
	string yiygzjalmtt;
	int oihjjzvacexd;
	string kbuhnob;
	int aujmkio;

	void ykkmgseaqmuylsywnjje(int ynzjdo, bool fvqvo);
	void lqnevfidolnrguqtlcfwt(string vkagdw);
	double xqagobwgblsbhyxtsmwnp(bool bxeebxcvwu, bool zpaypndfsadld, int zbbnlofifnh, double brhhggpmzccgxul);
	double jclfopszcybrkphukipqhngzc(double mzticzfisn, int vedtt, double iqkyrwwj, double beqdefybjrwlox, string laaxvvdidvk);
	string ekpydwrqnri(int mhxatdqidt, int uawbwvgrertsww, double mrwcbjkmtdbich, bool gszvuf, bool bxisxutnflcwmah, bool rgxbsc, string eujjow);
	void exmgmvcysrhggaqvpc(bool vzjcgzihejdno, int nnxgj, bool oxgpncxweuvgbpr, double lzxjoatl, double hwynnt, double zcodvx, double hhgtxxxvfqsyx, double aqvdgp, int avoqigrkaa);

};


void znyomje::ykkmgseaqmuylsywnjje(int ynzjdo, bool fvqvo) {
	string wlqxglbijpfvwc = "xxnesgaoohzsefvvxfmupvcqyzafpovgpwvkratyktqijlkmxpyngrjgblxbubiddlgtkixbgnobhaapbjcqkamcbjuq";
	int fieszny = 2454;
	bool jdmlcthbifri = false;
	int xihijrmsskb = 249;
	string rvozebksmwanv = "srdrdtxgeoswhzkssyjvguopfghrlnhoqokczsjofputiumwbfylgzudukbnajjfynfmpkr";
	if (2454 == 2454) {
		int skrklqe;
		for (skrklqe = 72; skrklqe > 0; skrklqe--) {
			continue;
		}
	}
	if (249 == 249) {
		int izopu;
		for (izopu = 29; izopu > 0; izopu--) {
			continue;
		}
	}
	if (249 == 249) {
		int eyhgwoidpl;
		for (eyhgwoidpl = 10; eyhgwoidpl > 0; eyhgwoidpl--) {
			continue;
		}
	}
	if (false != false) {
		int bphtbicxds;
		for (bphtbicxds = 11; bphtbicxds > 0; bphtbicxds--) {
			continue;
		}
	}

}

void znyomje::lqnevfidolnrguqtlcfwt(string vkagdw) {
	string sxjpnflstorho = "cmnw";
	bool rzhknwuscwef = false;
	string gjaeywx = "wrssldcuekrtqckuysdedltparlzmdhec";
	bool dolcts = true;
	int fscez = 3129;
	double vahszloaq = 14424;
	if (string("cmnw") != string("cmnw")) {
		int oajziusrzr;
		for (oajziusrzr = 11; oajziusrzr > 0; oajziusrzr--) {
			continue;
		}
	}
	if (false != false) {
		int cg;
		for (cg = 29; cg > 0; cg--) {
			continue;
		}
	}

}

double znyomje::xqagobwgblsbhyxtsmwnp(bool bxeebxcvwu, bool zpaypndfsadld, int zbbnlofifnh, double brhhggpmzccgxul) {
	double woqmpdnqpozam = 25208;
	int hqmzgwuflb = 625;
	bool pdruiymbdfuzmih = false;
	int dizebbsggikhic = 7265;
	string hklwieooejuhxc = "fvpidqvy";
	int weyuumzrpfflfgy = 2376;
	string newts = "ihumgdbsmzgxvwdsfwmmbmxsmiydqkrcummshtsnlkjkdskkdinemzjjdfgghuotwwnxeziihpkcmqouvzyrwqhxxrnnzcdkyheh";
	bool dujlxtedtubw = false;
	string iuaaxqtaab = "htogjjppkuncprfqyhogkazsqbevfigopyngssdfjiqpmdlsvjshzvtuhozkibkrldd";
	bool brsowgbseutkx = false;
	if (false == false) {
		int cd;
		for (cd = 50; cd > 0; cd--) {
			continue;
		}
	}
	if (false != false) {
		int iydcwbqoku;
		for (iydcwbqoku = 66; iydcwbqoku > 0; iydcwbqoku--) {
			continue;
		}
	}
	if (false == false) {
		int tuf;
		for (tuf = 29; tuf > 0; tuf--) {
			continue;
		}
	}
	if (string("ihumgdbsmzgxvwdsfwmmbmxsmiydqkrcummshtsnlkjkdskkdinemzjjdfgghuotwwnxeziihpkcmqouvzyrwqhxxrnnzcdkyheh") != string("ihumgdbsmzgxvwdsfwmmbmxsmiydqkrcummshtsnlkjkdskkdinemzjjdfgghuotwwnxeziihpkcmqouvzyrwqhxxrnnzcdkyheh")) {
		int lmhfalbvsu;
		for (lmhfalbvsu = 88; lmhfalbvsu > 0; lmhfalbvsu--) {
			continue;
		}
	}
	if (string("ihumgdbsmzgxvwdsfwmmbmxsmiydqkrcummshtsnlkjkdskkdinemzjjdfgghuotwwnxeziihpkcmqouvzyrwqhxxrnnzcdkyheh") != string("ihumgdbsmzgxvwdsfwmmbmxsmiydqkrcummshtsnlkjkdskkdinemzjjdfgghuotwwnxeziihpkcmqouvzyrwqhxxrnnzcdkyheh")) {
		int uctck;
		for (uctck = 8; uctck > 0; uctck--) {
			continue;
		}
	}
	return 31889;
}

double znyomje::jclfopszcybrkphukipqhngzc(double mzticzfisn, int vedtt, double iqkyrwwj, double beqdefybjrwlox, string laaxvvdidvk) {
	int vporduskwajik = 615;
	int soxcavieq = 4413;
	bool txapcqqtp = false;
	int jlvxve = 109;
	if (109 != 109) {
		int uzmflhslbw;
		for (uzmflhslbw = 98; uzmflhslbw > 0; uzmflhslbw--) {
			continue;
		}
	}
	if (false != false) {
		int nmtuagw;
		for (nmtuagw = 27; nmtuagw > 0; nmtuagw--) {
			continue;
		}
	}
	if (4413 == 4413) {
		int qr;
		for (qr = 62; qr > 0; qr--) {
			continue;
		}
	}
	return 84701;
}

string znyomje::ekpydwrqnri(int mhxatdqidt, int uawbwvgrertsww, double mrwcbjkmtdbich, bool gszvuf, bool bxisxutnflcwmah, bool rgxbsc, string eujjow) {
	int xrtwlbeq = 2252;
	int jlfvzw = 5200;
	string fevzwvccjhlx = "ptuhvsugcj";
	double oxaweoozkeuo = 25028;
	int vkudnztqzgg = 707;
	string vakhl = "nhttrhkvznxsucgibxyhikgpjoowageueoamrvkuyrrhwcmvmnwhyhovpnpecbtakqyim";
	if (25028 == 25028) {
		int lzmbvxbd;
		for (lzmbvxbd = 69; lzmbvxbd > 0; lzmbvxbd--) {
			continue;
		}
	}
	if (2252 != 2252) {
		int rl;
		for (rl = 86; rl > 0; rl--) {
			continue;
		}
	}
	if (2252 == 2252) {
		int zr;
		for (zr = 19; zr > 0; zr--) {
			continue;
		}
	}
	return string("pbu");
}

void znyomje::exmgmvcysrhggaqvpc(bool vzjcgzihejdno, int nnxgj, bool oxgpncxweuvgbpr, double lzxjoatl, double hwynnt, double zcodvx, double hhgtxxxvfqsyx, double aqvdgp, int avoqigrkaa) {
	bool qcmaffmpgab = false;
	int fsxkqj = 5111;
	if (5111 != 5111) {
		int riatfocre;
		for (riatfocre = 53; riatfocre > 0; riatfocre--) {
			continue;
		}
	}

}

string znyomje::utjzmvpcwqqnd(double cfhxyjxje, double dthvzrofqo, string hnmhejrhjjgbllv, string tmcopt) {
	bool rqmqtuibcqps = true;
	string ygxzuhhcgp = "bmukxi";
	if (string("bmukxi") != string("bmukxi")) {
		int vjwca;
		for (vjwca = 96; vjwca > 0; vjwca--) {
			continue;
		}
	}
	if (string("bmukxi") != string("bmukxi")) {
		int xldvxlaig;
		for (xldvxlaig = 30; xldvxlaig > 0; xldvxlaig--) {
			continue;
		}
	}
	if (true == true) {
		int qnkusprcrs;
		for (qnkusprcrs = 89; qnkusprcrs > 0; qnkusprcrs--) {
			continue;
		}
	}
	return string("kva");
}

bool znyomje::jtxjvehnki() {
	double eeokupw = 4098;
	bool zoeuhmmr = false;
	string osijo = "rbntlgdlbdd";
	string rzyfhe = "dsnblcxakggksmcqhtzvwd";
	bool autyhdkuys = false;
	double iooloxaas = 63405;
	string zcehuqozqoqiml = "snhcupnoxrzaroeybopabeirgeihduddqkkutwdryabzzaxeh";
	bool zkaywjubmpmlig = true;
	if (false == false) {
		int gwewtkndwt;
		for (gwewtkndwt = 22; gwewtkndwt > 0; gwewtkndwt--) {
			continue;
		}
	}
	if (string("snhcupnoxrzaroeybopabeirgeihduddqkkutwdryabzzaxeh") != string("snhcupnoxrzaroeybopabeirgeihduddqkkutwdryabzzaxeh")) {
		int ufomirdhz;
		for (ufomirdhz = 47; ufomirdhz > 0; ufomirdhz--) {
			continue;
		}
	}
	if (string("snhcupnoxrzaroeybopabeirgeihduddqkkutwdryabzzaxeh") != string("snhcupnoxrzaroeybopabeirgeihduddqkkutwdryabzzaxeh")) {
		int lzfntlny;
		for (lzfntlny = 72; lzfntlny > 0; lzfntlny--) {
			continue;
		}
	}
	return false;
}

string znyomje::chsvkqcfpaj(bool xzdpgdzbqbmy, string frgsujwcqxjpimi, double rvakswspwaj, int qdgfnklbay, bool owdsnyaa, string epjybij, double rkrfdezlrfuwpin, string ymavjmquejil, string xgufhtpwrndqis, double wzhnmib) {
	double vyffyymn = 17998;
	if (17998 != 17998) {
		int sn;
		for (sn = 53; sn > 0; sn--) {
			continue;
		}
	}
	if (17998 != 17998) {
		int zolbqi;
		for (zolbqi = 9; zolbqi > 0; zolbqi--) {
			continue;
		}
	}
	if (17998 != 17998) {
		int trer;
		for (trer = 86; trer > 0; trer--) {
			continue;
		}
	}
	if (17998 != 17998) {
		int luqq;
		for (luqq = 36; luqq > 0; luqq--) {
			continue;
		}
	}
	return string("");
}

bool znyomje::yfilpznwsaqoumkjdnovzv() {
	bool xbnxulcjrkbchoq = true;
	bool qtcste = true;
	int tnhgkw = 1487;
	bool eluxcynqqq = false;
	bool ejhnuyibtsg = false;
	bool hlnserhiwpltdl = false;
	if (1487 == 1487) {
		int jcw;
		for (jcw = 35; jcw > 0; jcw--) {
			continue;
		}
	}
	if (false != false) {
		int cxsnnpy;
		for (cxsnnpy = 80; cxsnnpy > 0; cxsnnpy--) {
			continue;
		}
	}
	if (false == false) {
		int yjr;
		for (yjr = 24; yjr > 0; yjr--) {
			continue;
		}
	}
	if (true != true) {
		int orhioiu;
		for (orhioiu = 8; orhioiu > 0; orhioiu--) {
			continue;
		}
	}
	if (false == false) {
		int pfkbjwkkeg;
		for (pfkbjwkkeg = 98; pfkbjwkkeg > 0; pfkbjwkkeg--) {
			continue;
		}
	}
	return false;
}

znyomje::znyomje() {
	this->yfilpznwsaqoumkjdnovzv();
	this->utjzmvpcwqqnd(10022, 6142, string("oluatskkfvvmsyqeygcquchrlbinjnbthmmrlecc"), string("ovtefvlgxnfjbjaixtquzfsvbjbntktonyvmmdmhrzwtphrjbulokiuneeczcvusalfckoccufuebbfhxfgdffivupxe"));
	this->jtxjvehnki();
	this->chsvkqcfpaj(true, string("bxyqcmdrhpohakqkebtpyizqkoljvw"), 11716, 5877, false, string("ktdyfowyvgqqwaccdpqwsnxhkdigkamfurasxesttkwkenfaahlgmqkokgxvxtzzoiednfh"), 36239, string("msvjdinwhdydqjebecvlrlkukdtqenusqmlkyouoqqj"), string("oemcycvlrbgfmnddiigbjuulvbqtzjckmpmcnvrz"), 39157);
	this->ykkmgseaqmuylsywnjje(5416, true);
	this->lqnevfidolnrguqtlcfwt(string("qjidxccpxtoxcyizvxwyjuvzntknivqroluzvzihqlazlnhxfopogytoewibyzxqxjqwfv"));
	this->xqagobwgblsbhyxtsmwnp(false, false, 212, 16117);
	this->jclfopszcybrkphukipqhngzc(52803, 141, 59808, 27654, string("t"));
	this->ekpydwrqnri(3608, 5420, 88, true, false, true, string("qxxtneeuzkagryzyyvffighvmxfjbgtgojqphbozvxbmwjydolhenntbudtifmelciyxjtpkmbjdclctrp"));
	this->exmgmvcysrhggaqvpc(false, 4841, true, 43921, 11109, 27339, 26613, 15884, 6456);
}
