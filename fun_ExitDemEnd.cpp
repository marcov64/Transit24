//#define NO_POINTER_INIT
#include "fun_head_fast.h"
#include "s-adapt.h"
//extern char msg[]; // deprecated in LSD 8.0
//char msg[ 5000 ];

#define NOCHECKS

MODELBEGIN


EQUATION("CheckCashAdjustments")
/*
Control all cash adjustments
*/
#ifdef NOCHECKS
END_EQUATION(-1);
#endif

v[0]=0;
CYCLE(cur, "Firm")
 v[0] += VS(cur, "CashFAdjustment");

v[1]=0;
CYCLE(cur, "Class")
 v[1] += VS(cur, "CashCAdjustment");

v[2]=0;
CYCLE(cur, "KFirm")
 v[2] += VS(cur, "CashKFAdjustment");

v[3]=0;
CYCLE(cur, "EnergyFirm")
 v[3] += VS(cur, "CashEFAdjustment");

v[4]=v[0]+v[1]+v[2]+v[3];
RESULT(v[4] )


EQUATION("CheckCapitalStock")
/*
Ensure that K sector revenues match the net new capital in FD and EN sector
*/

#ifdef NOCHECKS
END_EQUATION(-1);
#endif

v[0] = V("SumCapitalPaymentEF");
v[1] = V("SumCapitalPaymentF");
v[2] = V("SumOutstandingDebtEFCh");
v[3] = V("SumOutstandingDebtFCh");
v[4] = V("SumKRevenues");
v[6] = V("SumScrappedPrincipal");

v[5]=v[4]-v[3]-v[2]-v[1]-v[0]-v[6];

if( t>1 && v[4]!=0&& abs(v[5]/v[4])>0.00001)
	INTERACT("Err. CheckCapitalStock", v[5]);
RESULT(v[5] )


EQUATION("MegaCheckGov")
/*
Check flows and stocks of gov. TO BE COMPLETED
*/

v[0] = V("GovernmentFund");
v[1] = VL("GovernmentFund", 1);
v[2] = V("TaxRevenues");


RESULT(1 )


EQUATION("MegaCheckC")
/*
Check flows and stocks of demand
*/


v[0] = V("SumExpenditure");
v[1] = V("TotIncome");
v[2] = V("ChTotalDepositsC");

v[3]=v[1]-v[0];
v[4]=v[3]-v[2];

v[34]=max(v[0],v[1]);
v[33]=abs(v[4]/v[34]);

if(t>1 && v[33]>0.0000001)
 INTERACT("Error MegaCheckC", v[4]);

RESULT(v[4] )

EQUATION("MegaCheckK")
/*
Check flows and stocks of KSector
*/


v[0] = V("SumKRevenues");
v[1] = V("SumKLaborCost");
v[2] = V("SumKWagePrem");
v[3] = V("TotKEN")*V("PriceEN");
v[4] = V("SumOverdraftPaymentKF");
v[5] = V("SumInterestDepositsKF");
v[6] = V("SumTaxK");
v[7] = V("SumKOandMRevenue");

v[8]=v[0]+v[7]+v[5]; //plus
v[9]=v[1]+v[2]+v[3]+v[4]+v[6]; //minus
v[10] = V("ChTotalDepositsK");
v[12] = V("ChTotalAssetK");
v[11]=v[10]-v[12]-v[8]+v[9];//should be zero

v[14]=max(v[8],v[9]);
v[13]=abs(v[11]/v[14]);

if(t>1 && v[13]>0.0000001)
 INTERACT("Error MegaCheckK", v[11]);
RESULT(v[11] )

EQUATION("MegaCheckEN")
/*
Check flows and stocks of ENergy sector
*/


v[0] = V("SumRevenuesEF");
v[1] = V("SumWagePremEF");
v[2] = V("SumLaborCostEF");
v[3] = V("SumOverdraftPaymentEF");
v[4] = V("SumInterestDepositsEF");
v[5] = V("SumCapitalPaymentEF");
v[6] = V("SumInterestPaymentEF");
v[7] = V("SumSumKOandMExpenditure");
v[8] = V("SumTaxEF");
v[9] = V("TotExcessCost");
v[14] = V("SumCashEFCh");

v[17] = V("SumScrappedCapitalEN");

v[15]=V("SumTotAssetsEF")-VL("SumTotAssetsEF", 1);
v[16] = V("SumOutstandingDebtEFCh")-v[15]-v[17];
v[18] = V("CapitalDestroyedEF");

v[11]=v[0]+v[4];//plus
v[12]=v[1]+v[2]+v[3]+v[6]+v[7]+v[8]+v[9]; //minus ignoring capital repayment

v[13]=v[14]+v[12]-v[11]-v[16]+v[18];

v[34]=max(v[11],v[12]);
v[33]=abs(v[13]/v[34]);

if(t> 1 && v[33]>0.0000001)
 INTERACT("Error MegaCheckEN", v[13]);

RESULT(v[13] )



EQUATION("MegaCheckF")
/*
Check flows and stocks of final demand firms
*/


v[0] = V("SumMonetarySales");
v[1] = V("SumLaborCost");
v[2] = V("SumRdExpenditure");
v[3] = V("SumENCost");
v[4] = V("SumOverdraftPaymentF");
v[5] = V("SumInterestDepositsF");
v[6] = V("SumCapitalPaymentF");
v[7] = V("SumInterestPaymentF");
v[8] = V("SumWagePrem");
v[9] = V("SumTaxF");

v[10] = V("SumCashFCh");
v[15]=V("SumTotAssetsF")-VL("SumTotAssetsF", 1);
v[16] = V("RemovedAssetsF");
v[17] = V("SumScrappedCapitalF");
v[11] = V("SumOutstandingDebtFCh")-v[15]-v[17];

/******
v[20]=v[21]=v[22]=v[23]=0;
CYCLE(cur, "Firm")
{
	v[20]+=VS(cur,"OutstandingDebtF")-VLS(cur,"OutstandingDebtF", 1)-(VS(cur,"TotAssetsF")- VLS(cur,"TotAssetsF", 1));
	v[21]+=VS(cur,"CashF")-VLS(cur,"CashF", 1);
	v[22]+=VS(cur,"LaborCost");
	v[23]+=VS(cur, "ENCost");
}
*****/

v[12]=v[0]+v[5]; //plus
//v[13]=v[1]+v[2]+v[3]+v[4]+v[6]+v[7]+v[8]+v[9];//minus
v[13]=v[1]+v[2]+v[3]+v[4]+v[7]+v[8]+v[9];//minus removing capital payment

v[14]=v[10]+v[13]-v[12]-v[11]+v[16];
//LOG("%lf %lf %lf %lf\n", v[10],v[13],v[12],v[11]);

v[34]=max(v[13],v[12]);
v[33]=abs(v[14]/v[34]);

if(t>1 && v[33]>0.0000001)
 INTERACT("Error MegaCheckF", v[14]);

RESULT(v[14] )


EQUATION("ControlFirm")
/*
Micro version of megacheck
*/

#ifdef NOCHECKS
END_EQUATION(-1);
#endif

v[0] = V("MonetarySales");
v[1] = V("LaborCost");
v[2] = V("RdExpenditure");
v[3] = V("ENCost");
v[4] = V("OverdraftPaymentF");
v[5] = V("InterestDepositsF");
v[6] = V("CapitalPaymentF");
v[7] = V("InterestPaymentF");
v[8] = V("WagePrem");
v[9] = V("TaxF");
v[10] = V("CashF")-VL("CashF", 1);
v[11] = V("OutstandingDebtF")-VL("OutstandingDebtF", 1)-(V("TotAssetsF")- VL("TotAssetsF", 1));


v[12]=v[0]+v[5]; //plus
//v[13]=v[1]+v[2]+v[3]+v[4]+v[6]+v[7]+v[8]+v[9];//minus
v[13]=v[1]+v[2]+v[3]+v[4]+v[7]+v[8]+v[9];//minus removing capital payment

v[14]=v[10]+v[13]-v[12]-v[11];
RESULT(v[14] )

EQUATION("TotAssetsF")
/*
Comulative assets to the firm, computed as the cost of purchase
*/
v[0]=0;
CYCLE(cur, "Capital")
 v[0] += VS(cur, "KExpenditures");

RESULT(v[0] )

EQUATION("TotAssetsEF")
/*
Comulative assets to the energy firm, computed as the cost of purchase
*/
v[0]=0;
CYCLE(cur, "CapitalEF")
 v[0] += VS(cur, "KExpendituresEN");

RESULT(v[0] )


EQUATION("SumENCost")
/*
Sum of ENCost of final demand firms
*/
v[0]=0;
CYCLE(cur, "Firm")
 v[0] += VS(cur, "ENCost");
RESULT(v[0] )

EQUATION("SumTotAssetsEF")
/*
Some of energy firms' assets
*/
v[0]=0;
CYCLE(cur, "EnergyFirm")
 v[0] += VS(cur, "TotAssetsEF");

RESULT(v[0] )

EQUATION("SumTotAssetsF")
/*
Some of firms' assets
*/
v[0]=0;
CYCLE(cur, "Firm")
 v[0] += VS(cur, "TotAssetsF");

RESULT(v[0] )

EQUATION("TotAssetsFCh")
/*
Change in total assets of firms
*/
v[0] = V("TotAssetsF")- VL("TotAssetsF", 1);
RESULT(v[0] )

EQUATION("SumTotAssetsFCh")
/*
Some of firms' assets
*/
v[0]=0;
CYCLE(cur, "Firm")
 v[0] += VS(cur, "TotAssetsFCh");

RESULT(v[0] )


EQUATION("SumCapitalPaymentF")
/*
Sum of capital payment by FD firms
*/
v[0]=0;
CYCLE(cur, "Firm")
 v[0] += VS(cur, "CapitalPaymentF");
RESULT(v[0] )

EQUATION("SumCapitalPaymentEF")
/*
Sum of capital payment by EN firms
*/
v[0]=0;
CYCLE(cur, "EnergyFirm")
 v[0] += VS(cur, "CapitalPaymentEF");
RESULT(v[0] )

EQUATION("SumSumKOandMExpenditure")
/*
Sum of O&M for all EN firms
*/
v[0]=0;
CYCLE(cur, "EnergyFirm")
 v[0] += VS(cur, "SumKOandMExpenditure");
RESULT(v[0] )


EQUATION("SumKOandMExpenditure")
/*
Comment
*/
v[0]=0;
CYCLE(cur, "CapitalEF")
	{
	v[2] = VS(cur, "KENCapacity");
	if(v[2]>0)
  	v[0]+=VS(cur, "KOandMExpenditure");
	}

RESULT(v[0] )


EQUATION("AssignOandRExpenseEN")
/*
Distribute KOandMRevenue
*/

CYCLE(cur, "EnergyFirm")
{
	CYCLES(cur, cur1, "CapitalEF")
	{
	  v[4] = VS(cur1, "KENCapacity");
	  if(v[4]>0)
	  {
			v[0] = VS(cur1, "KOandMExpenditure");
			v[1] = VS(cur1, "IdCur");
			cur2 = SEARCH_CNDS(c, "IdKFirm", v[1]);
		  cur3=SEARCHS(cur2, "BankK");		
			INCRS(cur3, "KOandMRevenue", v[0]);
		}
	}

}

RESULT(1 )

EQUATION("SumKOandMRevenue")
/*
Compute the sum of Operation&Maintanance expenses
*/
//V("Production");
v[0]=0;
CYCLE(cur1, "KFirm")
	{
	VS(cur1, "KProductionFlow");
	cur=SEARCHS(cur1, "BankK");
	WRITES(cur, "KOandMRevenue",0);
	}
V("AssignOandRExpenseEN");
CYCLE(cur1, "KFirm")
	{
	cur=SEARCHS(cur1, "BankK");
	v[0]+=VS(cur, "KOandMRevenue");
	}
RESULT(v[0] )





EQUATION("ProfitEF")
/*
Energy firm profits
*/
V("PriceEN");

if(V("EFType")==0)
 v[0]=V("PriceENBrown");
else
 v[0]=V("PriceENGreen");
v[1]=V("EnergyProductionEF");
v[3]=V("LaborCostEF");
v[4]=V("OverdraftPaymentEF");/**/
v[5]=V("ExcessCostEF");
v[6] = V("InterestDepositsEF");/**/
//v[7]=V("CapitalCostEF");
v[9]=V("CapitalPaymentEF");
v[10]=V("InterestPaymentEF");/**/
v[11]=V("SumKOandMExpenditure");

RESULT(v[0]*v[1]-v[3]-v[4]-v[5]+v[6]-v[9]-v[10]-v[11])




EQUATION("DemographyEN")
/*
Comment
*/

V("StatTotalAssets");

v[0] = V("MinUtilization");
v[30] = V("TimeRepaymentEF");
v[21]=v[60]=0;
v[43] = V("minShareKEN");
WRITE("SumScrappedCapitalEN", 0);
v[44]=0;
CYCLE(cur, "EnergyFirm")
{ WRITES(cur, "pruned", 0);
	v[10]=VS(cur,"EFType");
	if(v[10]==0)
	 {
	 v[98]=V("MaxPPKAgeBrown");
	 v[96]=V("KOandMParaBrown");
	 }
	else
	 {
	 v[98]=V("MaxPPKAgeGreen"); 
	 v[96]=V("KOandMParaGreen");
	 }
	 
	v[3] = VS(cur, "NumPP");
  WRITES(cur, "PPremoved", 0);
  WRITES(cur, "ForcePurchaseKEN", 0); 
  CYCLE_SAFES(cur, cur1, "CapitalEF")
  	{
  		v[2] = VLS(cur1, "MAUtilizationPP",1);
  		v[20]=VS(cur1, "PrincipalEF");
  		v[4] = VS(cur1, "PPKAge");
  		v[5] = VS(cur1, "KENCapacity");
  		v[6] = VS(cur1, "MaxEnergyCapacity");

  		if(v[4]>v[30]*3)
  		 v[31]=1-pow(0.999,v[4]-v[30]*3); //prob. to die increasing with age being 0 when capital is just repaid and becoming 1 when age=4 x TimeRepayment
  		else
  		 v[31]=-1;


  		v[32]=0+pow(0.99,-(v[4]-(v[98]*1.75))); //convex function that starts at zero and increases quciker the older the capital is. Helps to determine the random scarpping of capital with an increasing chance the older the capital is.
  		v[33]=RND;
  	  v[34]=V("MinPPKAgeScrapPara");
  		v[35]=(v[98]*v[34]);//sets the earliest age at which capital may break randomly with an increasing chance

  		if(v[4]>0 && (v[2]<v[0] || v[4]>v[98] || (v[4]>v[35] && v[32]>v[33]) )) //final condition says capital is scrapped if it is above the min age for scrapping and if a random number is less than the number generated from convex function which increases the older the capital is. functions are calibrated so that overall there is around a 50% chance capital won't reach its potential lifespan, with scrapping more likely the older the capital gets
  		 {
        if(v[3]>1)
  		   {
        v[55]=VS(cur1,"KExpendituresEN");
        v[60]+=v[55];
        v[56]=V("KOandMParaBrown");
        v[58]=VS(cur1,"MaxPPKAgeBrown");
 		    if(V("ExitFlagEF")==1)
  		    {
  		     INTERACTS(cur1, "Scrap KEF", v[5]);
  		     INTERACTS(cur1, "Scrap PrincipalEF", v[20]);
  		    }
  		    
 		    v[3]=v[71]=INCRS(cur, "NumPP", -1);
        if(v[71]==0)
         INTERACTS(cur1, "Scrapping last KEN", v[55]);
 		    DELETE(cur1->hook);
 		    DELETE(cur1);
 			  v[21]+=v[20];
 		    v[1]=1;
 		    INCRS(cur, "PPremoved", v[20]);
 		    v[44]+=v[20];
 		    WRITES(cur, "pruned", 1);
 		   } //end of control on NumPP > 1
 		  else
  		  {
  		   //INTERACTS(cur, "Checazzo", v[3]);
  		   WRITES(cur, "ForcePurchaseKEN", 1); 
  		  } 
  		  } 
  		 }
  	}  
  
WRITE("SumScrappedCapitalEN", v[60]);
WRITE("SumScrappedPrincipal", v[44]);
cur1=SEARCHS(p->up,"Energy");
v[4]=V("probEntryEF");
v[40] = V("minTimeEntryEF");
v[41] = INCR("CounterEntryEF", 1);
if(RND<v[4] && t>1 && v[41]>v[40] )
 {
	v[1]=V_CHEATS(cur1,"EntryEF",p);
	WRITE("CounterEntryEF", 0);
 }
RESULT(v[21] )


EQUATION("KProductionFlow")
/*
Activity of the K producing firm
 */
//v[10]=t; //sprintf(msg, "\n KProductionFlow(%g)", v[10]); plog(msg);


cur = SEARCH("BankK");
WRITES(cur, "KRevenues", 0);

CYCLES(p->up->up,cur,"EnergyFirm")
	{
	VS(cur,"InvestmentDecisionEF");
	//sprintf(msg, "\n cycle KProd(%g)", v[10]); plog(msg);
	}

v[30]=0;

v[0]=V("KQ"); //production capacity of the firm
v[98] = V("KRatioVacancies");
if(v[98]>0)
 {
  v[99] = V("KExtraCapacity");
  v[97]=min(v[98]+1,v[99]);
  v[0]*=v[97];
 }

v[1]=V("NumOrders");
if(v[1]==0)
  END_EQUATION(0);

v[2]=v[0]/v[1]; //one way to determine the amount of K production capacity per order. Otherwise...

v[3]=0;
CYCLE(cur, "Order")
{
  v[4]=VS(cur,"KAmount"); //sprintf(msg, " KAmount(%g)\n", v[4]); plog(msg);
  v[5]=VS(cur,"KCompletion");
  v[3]+=v[4]-v[5];
}
cur5=SEARCH("BankK");
//cur5=SEARCHS(c,"BankK");
WRITES(cur5,"KRevenues",0);

CYCLE_SAFE(cur, "Order")
{//increase the level of advancement of the orders and, if completed, remove the order. Given the production capacity, devote it respecting oreders' order (first comes first go, which allows to respect the priority given by customers, on side, and to reduce the dofferences between the price agreed upon ordering and the price at which the kapital is sold)
  v[4]=VS(cur,"KAmount");
  v[5]=VS(cur,"KCompletion");
  v[6]=(v[4]-v[5]); // given the missing quantity of the current order
  //v[7]=v[6]*v[0]; //share of production capacity devoted to this order
  v[8]=min(v[0], v[4]-v[5]); //use the production capacity needed actually neded to produce the order, or exhaust here the production capacity (for the current period)
  INCRS(cur,"KCompletion",v[8]);
  v[0]=v[0]-v[8];
  v[5]=VS(cur,"KCompletion"); //update the completion level  in order to cancel the order if done
  if(v[5]>=v[4])
  {//order fulfilled. Either search for the ordering firm, or simply use the hook

    if(v[5]>0)
    {//stupid control needed to not be confused by the very initial object
      v[30]+=v[5]; // keep tracks of all fullfilled Orders: KproductionFlow
        if(VS(cur,"EnergyKOrder")==0)
        {
          if(cur->hook==NULL)
            INTERACT("hook NULL",v[0]);
          //        INCRS(cur->hook,"NumK",1); // hook should be the ordering firm
          //cur1=ADDOBJS(cur->hook,"Capital");
          //      cur1=cur->hook->add_an_object("Capital");
          //        if(t>7)
          //      INTERACTS(cur->hook, "PincoPallo", v[5]);
          if(VS(cur->hook, "flagNewEntrant")==1)
           {
            WRITELS(cur->hook,"CapitalStock",v[5],t);
            cur1 = SEARCHS(cur->hook, "Capital");

           }
          else
           cur1=ADDOBJS(cur->hook,"Capital");
          WRITELS(cur1,"K",v[5],t);
          WRITELS(cur1,"FDKCapacity",v[5]/V("CapitalIntens"),t);
          
          v[9]=VS(cur,"Kproductivity");
          WRITELS(cur1,"IncProductivity",v[9],t);

          // Incorporate KEfficiency in the vintage produced IncEfficiency
          v[90]=VS(cur,"KEfficiency");
          WRITELS(cur1,"IncEfficiency",v[90],t);

          WRITELS(cur1,"IncLearningK",0.1,t);
          WRITELS(cur1,"KAge",0,t);
          v[11]=VS(cur,"KP");
          WRITES(cur1,"KExpenditures",v[11]*v[4]);
          WRITELS(cur1,"PrincipalF",v[11]*v[4],t);
          WRITELS(cur->hook,"InvestmentSpending",v[4]*v[11],t);
          if(VS(cur->hook, "flagNewEntrant")==1)
           {
            cur4 = SEARCHS(cur->hook, "BankF");
            //WRITELS(cur4, "CashF", 0,t);
            //INTERACTS(cur, "K NewE", v[11]*v[4]);
            WRITES(cur->hook, "flagNewEntrant", 0);
           }
          WRITES(cur->hook,"Waiting",0); //tell the firms it has the new capital
          //sprintf(msg, "waiting zero(%g)\n", v[0]); plog(msg);
          SORTS(cur->hook,"Capital","IncProductivity", "DOWN");
          //INTERACTS(cur, "Firm K delivered", v[12]-v[4]*v[11]);
         } //end delivered to final good firm
        else
         {//delivery of a K for Energy firm

          if(cur->hook==NULL)
            INTERACT("hook NULL KEN",v[0]);

          v[50]=VS(cur,"IdClient");

          cur5=SEARCH_CND("IdFirmEN",-v[50]);

          if(cur5!=cur->hook)
            INTERACT("cur diff",v[50]);

           if(cur5==NULL)
            INTERACT("cur5 NULL",v[0]);

                   //cur7=SEARCH_CNDS(cur->hook, "CapitalEN", 0 ); // Remove the original K if any WRONG!!!!! It returns the first CapitalEN==0, from another FirmEN is cur->hook has none
          cur7=NULL;
          CYCLES(cur->hook, cur8, "CapitalEF")
          {//checks if there is a null capital in the firm, and only that firm
          	v[77] = VS(cur8, "CapitalEN");
          	if(v[77]==0)
          	{
          	 cur7=cur8;
          	 break;
          	}
          }


          if(cur7!=NULL)
          	cur1=cur7;
          else
            {
            if(cur->hook==NULL)
             INTERACTS(cur,"ERR212",v[0]);
            else
             {v[50]=VS(cur->hook, "IdFirmEN");
              //LOG("%d", (int)v[50]);
              }
            cur1=ADDOBJS(cur->hook,"CapitalEF"); // hook doesn"t work

            }
 //CHECK IT OUT. Before there was a comment after 'else', may be it disrupted the conditional statement           INTERACTS(cur1, "Created", v[50]);


          INCRS(cur->hook, "NumPP", 1);
          WRITES(cur->hook, "ForcePurchaseKEN", 0);
          WRITES(cur->hook, "TimeDeliveryPP", (double)t);
          //INCRS(cur5, "NumPP", 1);
          cur2 = ADDOBJS(cur->hook->up, "PP");
          WRITES(cur2, "IdEF", v[50]);

          //cur2 = ADDOBJS(cur5->up, "PP");
          cur1->hook=cur2;
          cur2->hook=cur1;
          if(VS(cur->hook, "EFType") == 0)
            v[20] = VS(cur->hook, "MultiplierCapacityENBrown");
          else
            v[20] = VS(cur->hook, "MultiplierCapacityENGreen");

          WRITES(cur1,"CapitalEN",v[5]*v[20]);
          WRITES(cur1,"EnergyProductionPP",0);
          WRITES(cur1, "FairEnergyPP", 0);


          v[9]=VS(cur,"Kproductivity");
//          v[21] = VS(cur->hook, "MultiplierProductivityEN");
          WRITES(cur1,"KENProductivity",v[9]);


          v[99]=VS(cur->hook,"EFType");
          if(v[99]==0)
           { // its a brown firm
            //INCRS(cur1->up,"HistoricalCapitalEN",v[5]);
            INCRS(cur1->up->up,"HistoricalCapitalEN",v[5]);
          	V("CallalphaBrown");
          	v[41]=V("alphaBrown");
          	WRITES(cur1,"alphaPP",v[41]);
           }
          else
           { // its a green firm
          	V("CallalphaGreen");
          	v[41]=V("alphaGreen");
          	WRITES(cur1,"alphaPP",v[41]);
           }

          v[42]=VS(cur->hook,"alphaSize");
					WRITES(cur1,"alphaSizePP",v[42]);



          WRITELS(cur1,"PPKAge",0,t);
          v[11]=VS(cur,"KP");
          v[12]=v[11]*v[4];
          WRITES(cur1,"KExpendituresEN",v[12]);


          cur5=SEARCH("BankK");

          v[56]=V("KOandMParaBrown");
          v[57]=V("KOandMParaGreen");
          v[58]=V("MaxPPKAgeBrown");
          v[59]=V("MaxPPKAgeGreen");

          if(v[99]==0)
           { // its a brown firm
           WRITES(cur1,"KOandMExpenditure",(v[12]*v[56])/v[58]);
           INCRS(cur5,"KOandMRevenue",(v[12]*v[56])/v[58]);
           v[98]=VS(cur5,"IdKFirm");
           WRITES(cur1,"IdCur",v[98]);
           }
          else
           { // its a green firm
          	WRITES(cur1,"KOandMExpenditure",(v[12]*v[57])/v[59]);
            INCRS(cur5,"KOandMRevenue",(v[12]*v[57])/v[59]);
            v[98]=VS(cur5,"IdKFirm");
            WRITES(cur1,"IdCur",v[98]);
           }




          //WRITELS(cur1,"PrincipalEF",0,t-1);
          WRITELS(cur1,"PrincipalEF",v[12],t);

          //INTERACT("aa",v[12]);
          //VS(cur->hook,"DebtEFL");
          //INCRS(cur->hook,"DebtEF",v[12]);

          WRITELS(cur->hook,"InvestmentSpendingEF",v[12],t);
          //INTERACTS(cur->hook, "Used", v[12]);
          //INTERACT("KP",v[12]);
          //WRITELS(cur->hook,"InvestmentSpendingEF",v[12],t); //sprintf(msg, "\n KProd InvestmentSpendingEF(%g)", v[12]); plog(msg); // hook doesn"t work
          //WRITELS(cur5,"InvestmentSpendingEF",v[12],t); //sprintf(msg, "\n KProd InvestmentSpendingEF(%g)", v[12]); plog(msg);


          WRITES(cur->hook,"WaitingEF",0); //tell the firms it has the new capital
          //sprintf(msg, "\n EN K delivered(%g)", v[12]); plog(msg);
          //WRITELS(cur5,"WaitingEF",0,t); //tell the firms it has the new capital

          //INTERACTS(cur, "Energy K delivered", v[12]-v[4]*v[11]);
         } //end delivered to energy firm

        cur5=SEARCH("BankK");
				//cur5=SEARCHS(c,"BankK");
        INCRS(cur5,"KRevenues",v[4]*v[11]);
        v[20]=INCR("NumOrders",-1);
        if(v[20]>0)
          DELETE(cur);
        else
        {
          WRITES(cur,"KAmount",0);
          WRITES(cur,"KCompletion",0);
          WRITES(cur,"TimeWaited",0);
          WRITES(cur,"Kproductivity",0);
          WRITES(cur,"KEfficiency",0);
          WRITES(cur, "IdClient", -1);
          cur->hook=NULL;
        }
      }//end control first toke object
   }//end order completed
  else
   {
    if(v[4]>0)
      INCRS(cur,"TimeWaited",1); // if orders remain non completed increase the time needed to go through future orders
   }

}

v[13]=min(V("KQ"),v[3]);

WRITE("KDelivered",v[30]);

//v[15]=V("KQ")-v[3];
//v[16]=v[15]-v[0];
//if(v[15]>0 && v[15]!=v[0])
//INTERACT("check the correspondence between production and KQ",v[16]);
//if(v[15]<0 && v[0]!=0)
//INTERACT("check the correspondence between production and KQ",v[0]);

RESULT(v[13] )



//Changed Equations

// ---------------------------------------------------------------------------
//Equation that:
//i) slightly modify the InitKENCapacity so that we can adjust the parameter MultiplierCapacityEN  without messing the amounts of capital the energy sector uses up (increasing the value decreases the amount of capital the energy sector requires to produce a unit of energy).

EQUATION("InitKENCapacity")
/*
Comment
*/
v[0] = V("CapitalEN");
v[1]=V("alphaPP");
v[2]=V("alphaSizePP");

RESULT(v[0]*v[1]*v[2]  )



// ---------------------------------------------------------------------------
//Equations that:
//i) slightly modify the WagePrem equations so that firms don't distribute profits if they have negative profits (becuase before they could distribute profits if they were making losses but had positive cash balances)

EQUATION("WagePremEF")
/*
Wage premia distributed, when available to all classes of executives.
*/


v[0]=V("ProfitEF");
v[1]=VL("CashEF",1);
v[50]=V("roPremia");
v[4]=V("roCash");

if(v[0]>0)
 v[5]=max(0,(v[0])*v[50]+v[1]*v[4]);
else
 v[5]=0;

v[3]=0;

if(v[5]>0)
 {
  CYCLE(cur, "LaborEF")
   {
    v[10]=VS(cur,"IdLaborEF");
    if(v[10]>1)
     {
      v[2]=VS(cur,"wageEF");
      if(VS(cur, "NumWorkersEF")>0) //distribute premia only to existing workers
        v[3]+=v[2];
     }
   }

  CYCLE(cur, "LaborEF")
   {
    v[10]=VS(cur,"IdLaborEF");
    if(v[10]>1)
     {v[2]=VS(cur,"wageEF");
     if(VS(cur, "NumWorkersEF")>0) //distribute premia only to existing workers
      {
       WRITES(cur,"PremiaEF",v[5]*v[2]/v[3]);
      // INCRS(cur->hook,"PremiaIncome",v[5]*v[2]/v[3]);
      }
     else
       WRITES(cur,"PremiaEF",0); 
     }
   }
 }
else
 {
   CYCLE(cur, "LaborEF")
   {
    WRITES(cur,"PremiaEF",0);
   }
 }

RESULT(v[5] )

EQUATION("DistributedExcessCash")
/*
Excess of cash distributed to workers
*/


v[20] = V("ThresholdExtraCash");
v[21] = VL("LiquidityCashRatio", 1);
v[22] = V("shareExtraCash");
if(v[21]>v[20])
 {
  v[23] = VL("CashF", 1);
  v[24]=v[22]*(v[23]-v[23]*v[20]);
 }
else
 v[24]=0;

RESULT(v[24] )


EQUATION("WagePrem")
/*
Wage premia distributed, when available to all classes of executives.
NEW
All employees, including blue collar workers, receive a share of extra cash in excess of given threshold
*/


v[0]=V("Profit");
v[7] = V("Windfall");
v[1]=VL("CashF",1);
v[50]=V("roPremia");
v[4]=V("roCash");

v[24] = V("DistributedExcessCash");

if(v[0]>0)
 v[5]=max(0,(v[0]-v[7])*v[50]+v[1]*v[4]);
else
 v[5]=0;

v[3]=v[33]=0;

if(v[5]>0 || v[24]>0)
 {
  v[60]=0;
  CYCLE(cur, "Labor")
   {
    v[10]=VS(cur,"IdLabor");
    v[2]=VS(cur,"wage");
    v[33]+=v[2];
    if(v[10]>1)
     {v[60]++;
      if(VS(cur, "NumWorkers")>0) //distribute premia only to existing workers
       v[3]+=v[2];
     }
    WRITES(cur, "Premia", 0);
   }
  if(v[60]<=1)
   v[5]=0; //do not distribute premia when there are only bluecollar workers
   
  CYCLE(cur, "Labor")
   {
    v[10]=VS(cur,"IdLabor");
    v[2]=VS(cur,"wage");
    if(VS(cur, "NumWorkers")>0) //distribute premia only to existing workers
    {
    if(v[10]>1 )
     {
      WRITES(cur,"Premia",v[5]*v[2]/v[3]);
     // INCRS(cur->hook,"PremiaIncome",v[5]*v[2]/v[3]);
     }
    INCRS(cur,"Premia",v[24]*v[2]/v[33]);
    //INCRS(cur->hook,"PremiaIncome",v[24]*v[2]/v[33]);
    }
   }
 }
 else
 {
   CYCLE(cur, "Labor")
   {
    WRITES(cur,"Premia",0);
   }
 }

RESULT(v[5]+v[24] )


EQUATION("KWagePrem")
/*
Wage premia distributed, when available to all classes of executives.
*/

//V("PayTime");

v[0]=V("KProfit");
v[1]=VL("CashKF",1);
v[50]=V("roPremia");
v[4]=V("roCash");

if(v[0]>0)
 v[5]=max(0,(v[0])*v[50]+v[1]*v[4]);
else
 v[5]=0;

v[3]=0;
if(v[5]>0)
 {
  CYCLE(cur, "KLabor")
   {
    v[10]=VS(cur,"IdKLabor");
    if(v[10]>1)
     {
      v[2]=VS(cur,"KWage");
      v[3]+=v[2];
     }
   }

  CYCLE(cur, "KLabor")
   {
    v[10]=VS(cur,"IdKLabor");
    if(v[10]>1)
     {v[2]=VS(cur,"KWage");
      WRITES(cur,"KPremia",v[5]*v[2]/v[3]);
     // INCRS(cur->hook,"PremiaIncome",v[5]*v[2]/v[3]);
     }
   }
 }
else
 {
   CYCLE(cur, "KLabor")
   {
    WRITES(cur,"KPremia",0);
   }
 }

RESULT(v[5] )



// ---------------------------------------------------------------------------
//Equations that:
//i) clean up the equations that determine the exit of firms firm
//ii) fix exit equations os they depend on free cash flows before interest paymnents and also smoothed debt, to prevent sudden decrease in RRoK when firms increase borrowing (which could lead to exit)


EQUATION("IncomeBeforeDebtPaymentsF")
/*
*/
v[0]=V("MonetarySales");
v[2]=V("LaborCost");
v[3]=V("RdExpenditure");
v[4]=V("ENCost");
v[8]=V("InterestDepositsF");

v[5]=v[0]-v[2]-v[3]-v[4]+v[8];
RESULT(v[5] )


EQUATION("IncomeBeforeDebtPaymentsEF")
/*
*/
V("PriceEN");
if(V("EFType")==0)
 v[0]=V("PriceENBrown");
else
 v[0]=V("PriceENGreen");

v[1]=V("EnergyProductionEF");
v[3]=V("LaborCostEF");
v[5]=V("ExcessCostEF");
v[6] = V("InterestDepositsEF");

v[7]=v[0]*v[1]-v[3]-v[5]+v[6];
RESULT(v[7])


EQUATION("IncomeBeforeDebtPaymentsKF")
/*
*/
V("KNbrEngineers"); // compute first the number of engineers which use the past value of cumulated profits
V("KProductionFlow");

v[0]=V("KRevenues");
v[2]=V("KENCost");
v[3]=V("KLaborCost");
v[8]=V("InterestDepositsKF");
v[4]=V("KOandMRevenue");

v[5]=v[0]-v[2]-v[3]+v[8]+v[4];
RESULT(v[5] )


EQUATION("sIncomeBeforeDebtPaymentsF")
/*
*/
v[1]=VL("sIncomeBeforeDebtPaymentsF",1);
v[2]=V("IncomeBeforeDebtPaymentsF");

v[3]=v[1]*0.95+0.05*v[2];
RESULT(v[3] )


EQUATION("sIncomeBeforeDebtPaymentsEF")
/*
*/
v[1]=VL("sIncomeBeforeDebtPaymentsEF",1);
v[2]=V("IncomeBeforeDebtPaymentsEF");

v[3]=v[1]*0.95+0.05*v[2];
RESULT(v[3] )


EQUATION("sIncomeBeforeDebtPaymentsKF")
/*
*/
v[1]=VL("sIncomeBeforeDebtPaymentsKF",1);
v[2]=V("IncomeBeforeDebtPaymentsKF");

v[3]=v[1]*0.95+0.05*v[2];
RESULT(v[3] )


EQUATION("LiabilitiesEF")
/*
*/
v[36]= -1 * V("CashEF");
v[34]=V("OutstandingDebtEF");

v[37]=v[36] + v[34];

if(v[37]<0.000)
 v[38]=0.000;
else
 v[38]=v[36]+v[34];

RESULT(v[38] )


EQUATION("LiabilitiesF")
/*
*/
v[36]= -1* V("CashF");
v[34]=V("OutstandingDebtF");

v[37]=v[36] + v[34];

if(v[37]<0.0001)
 v[38]=0.0001;
else
 v[38]=v[36]+v[34];

RESULT(v[38] )



EQUATION("LiabilitiesKF")
/*
*/
v[36]=-1 * V("CashKF");
v[34]=0;

v[37]=v[36] + v[34];


if(v[37]<0.0001)
 v[38]=0.0001;
else
 v[38]=v[36];

RESULT(v[38] )


EQUATION("sLiabilitiesEF")
/*
*/
v[1]=VL("sLiabilitiesEF",1);
v[2]=V("LiabilitiesEF");

v[3]=v[1]*0.95+0.05*v[2];
RESULT(v[3] )


EQUATION("sLiabilitiesF")
/*
*/
v[1]=VL("sLiabilitiesF",1);
v[2]=V("LiabilitiesF");

v[3]=v[1]*0.95+0.05*v[2];
RESULT(v[3] )


EQUATION("sLiabilitiesKF")
/*
*/
v[1]=VL("sLiabilitiesKF",1);
v[2]=V("LiabilitiesKF");

v[3]=v[1]*0.95+0.05*v[2];
RESULT(v[3] )



EQUATION("RRoKEF")
/*
*/
v[1]=V("sLiabilitiesEF");
v[2]=V("sIncomeBeforeDebtPaymentsEF");

if(v[1]<0.001)
 v[1]=0.001;

v[3]=v[2]/v[1];
RESULT(v[3] )



EQUATION("RRoK")
/*
*/
v[1]=V("sLiabilitiesF");
v[2]=V("sIncomeBeforeDebtPaymentsF");

if(v[1]<0.001)
 v[1]=0.001;

v[3]=v[2]/v[1];
RESULT(v[3] )



EQUATION("RRoKKF")
/*
*/
v[1]=V("sLiabilitiesKF");
v[2]=V("sIncomeBeforeDebtPaymentsKF");

if(v[1]<0.001)
 v[1]=0.001;

v[3]=v[2]/v[1];
RESULT(v[3] )



EQUATION("sProfitEF")
/*
Smoothed ProfitEF
*/
v[1]=V("ProfitEF");
if(t>1)
 v[0]=VL("sProfitEF",1);
else
 v[0]=v[1];

v[2]=v[0]*0.95+0.05*v[1];
RESULT(v[2] )



EQUATION("sProfit")
/*
Smoothed ProfitF
*/
v[0]=VL("sProfit",1);
v[1]=V("Profit");
v[2]=v[0]*0.95+0.05*v[1];

RESULT(v[2] )



EQUATION("sProfitKF")
/*
Smoothed ProfitKF
*/
v[0]=VL("sProfitKF",1);
v[1]=V("KProfit");

v[2]=v[0]*0.95+0.05*v[1];
RESULT(v[2] )






// ---------------------------------------------------------------------------
//Equations that make HHs include bank capital as part of their wealth, so that we don't get such a sudden drop in demand when the bank is recapitalised

EQUATION("Expenditure")
/*
Total money spent by consumers, computed as a combination of past consumption and available resources (from wages and stock options)
*/
v[0]=VL("Expenditure",1);
if(t==1)
 END_EQUATION(v[0]); //at the first step do not
v[1]=V("Consumption");
v[2]=V("aEx");
v[10]=v[0]*v[2]+(1-v[2])*(v[1]);
if(t==1)
 v[10]=v[1];

v[11]=max(v[10],0);
v[15] = V("brakeExpenditure");
if(v[0]>v[11])
 v[11]=v[0]*v[15]+(1-v[15])*v[11];

v[12] = VL("CashC", 1);
if(v[11]>v[12])
 {
 //INTERACT("STOP EXP", v[10]);
 v[11]=v[12];
 }
if(v[12]>0)
 WRITE("RatioExpCash", v[0]/v[12]);
else
 WRITE("RatioExpCash", 0);
RESULT(v[11] )

EQUATION("ReliableIncome")
/*
Income considered as reliably received on which to base consumption decision
It is computed as wage income
*/
V("Income");
v[0] = V("WageIncome");
RESULT(v[0] )


EQUATION("Consumption")
/*
Desired level of consumption
*/

v[0]=VL("ReliableIncome",1);

v[1]=V("SavingRate");
//v[3]=VL("HHWealth",1);
v[3]=VL("CashC",1);

v[4]=V("AutonomousC");
v[5]=V("aCashC");
v[6] = VL("Individuals",1);
v[7] = V("MovAvPrice");

v[12]= (v[4]) + (v[0] + v[5]*v[3] )*(1-v[1]) ;
//v[12]= (v[4]) + (v[0]*(1-v[1])) + (v[5]*v[3]);
WRITE("AutComp", v[6]*v[4]*v[7]);
if(v[3]<0)
	{
	 LOG("Zero C. %d", (int)v[6]);
	 INTERACT("ZERO", v[6]);
	 v[12]=0;

	}
RESULT(v[12] )



EQUATION("HHWealth")
/*
HH own the bank in proportion to their wealth
*/
v[1] = V("CashC");
v[2] = V("BankCapital");
v[3] = V("CashCShare");

v[4] = v[1] + (v[2]*v[3]);
RESULT(v[4] )



EQUATION("CashCShare")
/*
Share of CashC held by each class
*/
v[1] = V("SumCashC");
v[2] = V("CashC");

v[3] = v[2]/v[1];
RESULT(v[3] )





//END OF NEW EQUATIONS



//------------------------------------------------------------
//OLD EQUATIONS


EQUATION("sInterestEF")
/*
Total interest to pay on the loan
*/
v[0] = VL("PrincipalEF",1);
v[1] = VL("sInterestRate",1);

RESULT(v[0]*v[1] )



EQUATION("sInterestPaymentEF")
/*
Interest payments on outstanding debt
*/
v[0]=0;
CYCLE(cur, "CapitalEF")
{
	v[0]+=VS(cur, "sInterestEF");
}

RESULT(v[0] )



EQUATION("sCapitalCostEF")
/*
Cost of repayment loans
*/
v[0]=V("CapitalPaymentEF");
v[1]=V("sInterestPaymentEF");/**/

RESULT(v[0]+v[1] )


EQUATION( "VacancyRate" )
/*
number of vacancies over employment
*/
v[0]=V("TotVacancies");
v[1]=V("NbrWorkers");
RESULT(v[0]/v[1] )


EQUATION( "UnitELdiff" )
/*
Difference between UnitEnergyCost and UnitLaborCost, must always be negative
*/
v[0]=V("UnitEnergyCost");
v[1]=V("UnitLaborCost");
RESULT(v[0]-v[1] )


EQUATION( "dGDP" )
/*
Gross domestic product (log) growth rate
*/
RESULT(t>1 ? log(V("GDP")+1) - log(VL("GDP",1)+1) : 0)


EQUATION("StatFirms")
/*
*/
v[11]=v[12]=0;
CYCLE(cur,"Firm")
	{
	 v[1]=VS(cur,"MonetarySales");
	 v[2]=VS(cur,"GOS");
	 v[11]+=v[1]; // sum of MonetarySales
	 v[12]+=v[2]; // sum of GOS
	}
WRITE("FirmsGOSShare", v[12]/v[11]); //UNRECOGNISE GRAMMAR ERROR
RESULT(1 )


EQUATION("StatEF")
/*
*/
v[20]=v[30]=v[21]=v[31]=0;
CYCLE(cur,"EnergyFirm")
	{
	v[0]=VS(cur,"EFType");
	if(v[0]==0)
		{ // it's a brown firm
		 v[1]=VS(cur,"RevenuesEF");
		 v[2]=VS(cur,"GOSEF");
		 v[20]+=v[1]; // sum of brown revenues
		 v[30]+=v[2]; // sum of brown GOS
		}
	if(v[0]==1)
		{ // it's a green firm
		 v[11]=VS(cur,"RevenuesEF");
		 v[12]=VS(cur,"GOSEF");
		 v[21]=+v[11]; // sum of green revenues
		 v[31]+=v[12]; // sum of green GOS
		}
	}
v[20]==0 ? v[40]=0 : v[40]=v[30]/v[20];
v[21]==0 ? v[41]=0 : v[41]=v[31]/v[21];

WRITE("BrownGOSShare",v[40]);
WRITE("GreenGOSShare",v[41]);

RESULT(1 )









EQUATION("GOSKShare")
/*
GOS / MonetarySales
*/
v[0]=V("KRevenues");
v[1]=V("GOSK");
v[0]==0 ? v[10]=0 : v[10] = v[1]/v[0];
RESULT(v[10] )



EQUATION("GOSShare")
/*
GOS / MonetarySales
*/
v[0]=V("MonetarySales");
v[1]=V("GOS");
if(v[0]>0)
 v[2]=v[1]/v[0];
else
 v[2]=0;
RESULT(v[2] )


EQUATION("GOS")
/*
Gross Operating Surplus, difference between revenues, labor costs, energy costs and R&D
*/
//v[0]=V("Revenues");
v[0]=V("MonetarySales"); //flo
v[2]=V("LaborCost");
v[3]=V("RdExpenditure");
v[4]=V("ENCost");

v[10]=v[0]-v[2]-v[3]-v[4];
RESULT(v[10] )

EQUATION("GOSEF")
/*
Gross Operating Surplus of energy firm, difference between revenues and labor costs
*/

V("PriceEN");

if(V("EFType")==0)
 v[0]=V("PriceENBrown");
else
 v[0]=V("PriceENGreen");

v[1]=V("EnergyProductionEF");
v[2]=V("LaborCostEF");
v[10]=v[0]*v[1]-v[2];
RESULT(v[10])


EQUATION("GOSK")
/*
Gross Operating Surplus of KFirm, difference between revenues, labor costs and energy costs
*/
V("KNbrEngineers"); // compute first the number of engineers which use the past value of cumulated profits
V("KProductionFlow");
v[0]=V("KRevenues");
v[2]=V("KENCost");
v[3]=V("KLaborCost"); // includes engineers
v[4]=V("KOandMRevenue");

v[10]=v[0]-v[2]-v[3]+v[4]; // Added KOandMRevenue
RESULT(v[10] )



EQUATION("AvUtilisationGreen")
/*
Average utilisation rate of capital in the energy sector weighted by capacity
*/
v[4]=v[5]=0;
CYCLE(cur,"EnergyFirm")
	{
	 v[1]=VS(cur,"EFType");
	 if(v[1]==1)
	 {// it's a green firm
	  v[2]=VS(cur,"AvUtilization"); // average utilisation of PP, not weighted by their size
	  v[3]=VS(cur,"MaxEnergyCapacity");
	  v[4]+=v[2]*v[3];
	  v[5]+=v[3];
	 }
	}
if(v[5]==0)
	END_EQUATION(0);
RESULT(v[4]/v[5] )

EQUATION("AvUtilisationBrown")
/*
Average utilisation rate of capital in the energy sector weighted by capacity
*/
v[4]=v[5]=0;
CYCLE(cur,"EnergyFirm")
	{
	 v[1]=VS(cur,"EFType");
	 if(v[1]==0)
	 {// it's a brown firm
	  v[2]=VS(cur,"AvUtilization"); // average utilisation of PP, not weighted by their size
	  v[3]=VS(cur,"MaxEnergyCapacity");
	  v[4]+=v[2]*v[3];
	  v[5]+=v[3];
	 }
	}
if(v[5]==0)
	END_EQUATION(0);
RESULT(v[4]/v[5] )


EQUATION("AvUtilisationRate")
/*
Average utilisation rate of capital weighted by CapitalCapacity
*/
v[3]=v[4]=0;
CYCLE(cur,"Firm")
	{
	 v[1]=VS(cur,"Q");
	 v[2]=VS(cur,"CapitalCapacity");
	 v[3]+=v[1];
	 v[4]+=v[2];
	}

RESULT(v[3]/v[4] )


EQUATION("UtilisationRate")
/*
Utilisation rate of capital
*/
v[1]=V("Q");
v[2]=V("CapitalCapacity");
if(v[2]==0)
	END_EQUATION(0);
RESULT(v[1]/v[2] )


EQUATION("ExcessEnergyRatio")
/*
*/
v[1]=V("ExcessEnergy");
v[2]=V("TotEnergyConsumption");
RESULT(v[1]/v[2] )

EQUATION("TotSavings")
/*
Total savings as the difference between TotIncome and TotConsumption
*/
v[1]=V("TotIncome");
v[2]=V("TotConsumption");
RESULT(v[2]-v[1] )


EQUATION("SavingsIncomeRatio")
/*
Savings / Income
*/
v[1]=V("TotIncome");
v[2]=V("TotConsumption");
RESULT((v[2]-v[1])/v[1] )



EQUATION("StatKFirms")
/*
*/
v[3]=v[4]=v[13]=v[14]=v[23]=v[24]=v[50]=v[51]=v[52]=v[5]=v[15]=v[25]=v[6]=v[16]=v[26]=v[7]=v[17]=v[27]=v[8]=v[18]=v[28]=v[9]=v[19]=v[29]=v[97]=v[98]=v[99]=0;
CYCLE(cur,"KFirm")
	{
	 v[0]=VS(cur,"KType");

   if(v[0]==0) // Brown
  	 {
  	  v[2]=VS(cur,"KProductionFlow");
  	  v[1]=VS(cur,"KRevenues");
  	  v[3]+=v[1]; // Nominal Brown Investment
  	  v[4]+=v[2]; // Real Brown Investment
  	  v[5]+=VS(cur,"WaitTime");
  	  v[6]+=VS(cur,"GOSK");
  	  v[7]+=VS(cur,"WaitTime");
  	  v[8]+=VS(cur,"KPrice");
  	  v[9]+=VS(cur,"KLaborProductivity");
  	  v[99]+=1;
  	 }
   if(v[0]==1) // Green
  	 {
  	  v[12]=VS(cur,"KProductionFlow");
  	  v[11]=VS(cur,"KRevenues");
  	  v[13]+=v[11]; // Nominal Green Investment
  	  v[14]+=v[12]; // Real Green Investment
  	  v[15]+=VS(cur,"WaitTime");
  	  v[16]+=VS(cur,"GOSK");
  	  v[17]+=VS(cur,"WaitTime");
  	  v[18]+=VS(cur,"KPrice");
  	  v[19]+=VS(cur,"KLaborProductivity");
  	  v[98]+=1;
  	 }
   if(v[0]==2) // Final good
  	 {
  	  v[21]=VS(cur,"KRevenues");
  	  v[22]=VS(cur,"KProductionFlow");
  	  v[23]+=v[21]; // Nominal Firms Investment
  	  v[24]+=v[22]; // Real Firms Investment
  	  v[25]+=VS(cur,"WaitTime");
  	  v[26]+=VS(cur,"GOSK");
  	  v[27]+=VS(cur,"WaitTime");
  	  v[28]+=VS(cur,"KPrice");
  	  v[29]+=VS(cur,"KLaborProductivity");
  	  v[97]+=1;
  	 }
 	  CYCLES(cur, cur1, "Order")
     {
  		 if(v[0]==0) // Brown
  		 	{v[50]+=VS(cur1,"TimeWaited");
  		 	//sprintf(msg, "\n TimeWaited(%g)", v[50]); plog(msg);
  		 	}
  		 if(v[0]==1) // Green
 	  		{v[51]+=VS(cur1,"TimeWaited");}
 	   	 if(v[0]==2) // Final good
 	      {v[52]+=VS(cur1,"TimeWaited");}
     }
	}

v[90]=V("KConstPrice"); // normalize by artificial constant price of K

WRITE("BrownI",v[4]*v[90]);
WRITE("BrownInom",v[3]);
WRITE("GreenI",v[14]*v[90]);
WRITE("GreenInom",v[13]);
WRITE("FirmsI",v[24]*v[90]);
WRITE("FirmsInom",v[23]);

v[30]=V("GDPnom");
v[40]=V("GDP");

WRITE("BrownIGdpRatio",v[40]==0 ? 0 : v[4]/v[40]);
WRITE("BrownInomGdpRatio",v[30]==0 ? 0 : v[3]/v[30]);
WRITE("GreenIGdpRatio",v[40]==0 ? 0 : v[14]/v[40]);
WRITE("GreenInomGdpRatio",v[30]==0 ? 0 : v[13]/v[30]);
WRITE("FirmsIGdpRatio",v[40]==0 ? 0 : v[24]/v[40]);
WRITE("FirmsInomGdpRatio",v[30]==0 ? 0 : v[23]/v[30]);

WRITE("TimeWaitedBrown",v[50]);
WRITE("TimeWaitedGreen",v[51]);
WRITE("TimeWaitedFirms",v[52]);

WRITE("WaitTimeBrown",v[5]);
WRITE("WaitTimeGreen",v[15]);
WRITE("WaitTimeFirms",v[25]);

v[1]==0 ? v[60]=0 : v[60]=v[6]/v[1];
v[11]==0 ? v[61]=0 : v[61]=v[16]/v[11];
v[21]==0 ? v[62]=0 : v[62]=v[26]/v[21];
WRITE("BrownGOSKShare",v[60]);
WRITE("GreenGOSKShare",v[61]);
WRITE("FirmsGOSKShare",v[62]);

WRITE("BrownWaitTime",v[7]/v[99]);
WRITE("GreenWaitTime",v[17]/v[98]);
WRITE("FirmsWaitTime",v[27]/v[97]);

WRITE("BrownKprice",v[8]/v[99]);
WRITE("GreenKprice",v[18]/v[98]);
WRITE("FirmsKprice",v[28]/v[97]);

WRITE("BrownKProductivity",v[9]/v[99]);
WRITE("GreenKProductivity",v[19]/v[98]);
WRITE("FirmsKProductivity",v[29]/v[97]);


RESULT(1 )


EQUATION( "dGDP" )
/*
Gross domestic product (log) growth rate
*/
RESULT( T > 1 ? log( V( "GDP" ) + 1 ) - log( VL( "GDP", 1 ) + 1 ) : 0 )



EQUATION( "dGDPnom" )
/*
Nominal gross domestic product (log) growth rate
*/
RESULT( T > 1 ? log( V( "GDPnom" ) + 1 ) - log( VL( "GDPnom", 1 ) + 1 ) : 0 )



EQUATION("sGDPnom")
/*
Smoothed nominal GDP
*/
v[1]=V("GDPnom");
v[2]=VL("sGDPnom",1);

v[3]=v[2]*0.95+0.05*v[1];

RESULT(v[3] )


EQUATION("sInterestRate")
/*
*/
v[1]=V("InterestRate");
v[2]=VL("sInterestRate",1);

v[3]=v[2]*0.95+0.05*v[1];

RESULT(v[3] )


EQUATION("NbrEF")
/*
*/
v[0]=v[1]=0;
CYCLE(cur,"EnergyFirm")
	{
	if(VS(cur,"EFType")==0)
		{ // it's a brown firm
		v[0]+=1;
		}
	else
		{ // it's a green firm
		v[1]+=1;
		}
	}
WRITE("NbrBrown",v[0]);
WRITE("NbrGreen",v[1]);
RESULT(COUNT("EnergyFirm") )



EQUATION("TotBacklogRatio")
/*
*/
v[0]=V("SumBacklog");
v[1]=V("TotQ");
v[2]=v[0]/v[1];
if(v[1]==0)
	v[2]=0;
RESULT(v[2] )





EQUATION("MaxNumWorkers")
/*
*/
v[0]=VL("MaxNumWorkers",1);

if(t < 2)
	END_EQUATION(v[0]);

v[1]=V("PopGrowth");
//v[2]=VL("UnemploymentRate",1);
v[2]=VL("STUnemployment",1);

//v[10]=v[0]*(1+v[1]);
v[3]=V("UnempTarget");
v[4]=(-v[2]+v[3]);
if(v[4]<0)
	v[10]=v[0];
else
	v[10]=v[0]*(1-v[2]+v[3]);

//RESULT(v[10] )
RESULT(v[0] )//POPULATION STABLE, NOT INCREASING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!



EQUATION("TotBacklogRatio")
/*
*/
v[0]=V("SumBacklog");
v[1]=V("TotQ");
v[2]=v[0]/v[1];
if(v[1]==0)
	v[2]=0;
RESULT(v[2] )


EQUATION("ControlWorkers")
/*
*/
v[0]=V("NbrWorkers");
v[1]=V("TotLaborForce");
v[2]=V("TotKLaborForce");
v[3]=V("SumTotNumWorkersEF");
RESULT(v[0]-v[1]-v[2]-v[3] )



EQUATION("WorkersEFRatio")
/*
*/
v[0]=V("NbrWorkers");
v[1]=V("SumTotNumWorkersEF");
RESULT(v[1]/v[0] )


EQUATION("WorkersKRatio")
/*
*/
v[0]=V("NbrWorkers");
v[1]=V("TotKLaborForce");
RESULT(v[1]/v[0] )


EQUATION("WorkersRatio")
/*
*/
v[0]=V("NbrWorkers");
v[1]=V("TotLaborForce");
RESULT(v[1]/v[0] )


EQUATION("UnemploymentRate")
/*
*/
v[0]=V("MaxNumWorkers");
v[1]=V("NbrWorkers");
v[2]=(v[0]-v[1])/v[0];
if(v[2]<0)
	{
	WRITE("NegUnemp",v[2]);
	//INTERACT("neg. UnemploymentRate", v[2]);
	v[2]=0;
	}
else
	{
	WRITE("NegUnemp",0);
	}
WRITE("Unemployment",v[0]-v[1]);
RESULT(v[2] )


EQUATION("SumTotNumWorkersEF")
/*
*/

v[0]=0;
CYCLE(cur,"EnergyFirm")
	 v[0]+=VS(cur,"TotNumWorkersEF");

RESULT(v[0] )



EQUATION("TotNumWorkersEF")
/*
*/
v[0]=0;
CYCLE(cur,"LaborEF")
	{
	 v[0]+=VS(cur,"NumWorkersEF");
	}
RESULT(v[0] )


EQUATION("NbrLaborEF")
/*
*/
RESULT(COUNT("LaborEF") )

EQUATION("WaitingEFRatio")
/*
*/
v[0]=V("SumWaitingEF");
v[1]=V("NbrEF");
RESULT(v[0]/v[1] )


EQUATION("WaitingRatio")
/*
*/
v[0]=V("SumWaiting");
v[1]=V("NbrFirms");
RESULT(v[0]/v[1] )



EQUATION("SumWaiting")
/*
*/

RESULT(SUM("Waiting") )


EQUATION("SumInvestmentSpendingEF")
/*
*/
V("TotInvestment"); //ensure that all K firms performed their production flow

v[0]=0;
CYCLE(cur, "EnergyFirm")
	v[0] += VS(cur, "InvestmentSpendingEF");

RESULT(v[0] )



EQUATION("SumNbrOrders")
/*
*/
RESULT(SUM("NumOrders") )


EQUATION("CheckFinBalanceRatio")
/*
*/
v[1]=V("CheckFinBalance");
v[2]=V("TotalAssets");
RESULT(v[1]/v[2] )



EQUATION("NbrPP")
/*
*/
RESULT(COUNT("PP") )


EQUATION("SumCashCCh")
/*
*/
V("Trade");
V("FinancialTrading");
V("InvestmentDecision");
V("Production");
v[1]=V("SumCashC");
v[2]=VL("SumCashC",1);
RESULT(v[1]-v[2] )

EQUATION("SumCashFChXXX")
/*
Different from EN firms' equivalent indicator. Removed and replaced with the same structure as in the EN sector
*/
V("Trade");
V("FinancialTrading");
V("InvestmentDecision");
V("Production");
v[1]=V("SumCashF");
v[2]=VL("SumCashF",1);
RESULT(v[1]-v[2] )

EQUATION("SumCashFCh")
/*
Sum of changes in cash across all firms
*/
V("Trade");
V("FinancialTrading");
V("InvestmentDecision");
V("Production");

v[0]=0;
CYCLE(cur, "Firm")
{
	v[0] += VS(cur,"CashFCh");
}

RESULT(v[0] )


EQUATION("SumCashKFCh")
/*
*/
V("Trade");
V("FinancialTrading");
V("InvestmentDecision");
V("Production");
v[1]=V("SumCashKF");
v[2]=VL("SumCashKF",1);
RESULT(v[1]-v[2] )



EQUATION("FirstOrderEF")
/*
*/
v[0]=V("TotEnergyConsumption");
v[1]=V("FirstOrderEFFrac");

RESULT(v[0]*v[1] )


EQUATION("SumInterestPaymentF")
/*
*/
v[0]=0;
CYCLE(cur, "Firm")
{
	v[0] += VS(cur, "InterestPaymentF");
}

RESULT(v[0] )

EQUATION("SumInterestPaymentEF")
/*
*/
RESULT(SUM("InterestPaymentEF") )

EQUATION("SumKFirmControlCurrent")
/*
*/
RESULT(SUM("KFirmControlCurrent") )


EQUATION("SumFirmControlCurrent")
/*
*/
RESULT(SUM("FirmControlCurrent") )


EQUATION("SumEFirmControlCurrent")
/*
*/
RESULT(SUM("EFirmControlCurrent") )


EQUATION("SumInterestDepositsC")
/*
*/
RESULT(SUM("InterestDepositsC") )

EQUATION("SumInterestDepositsF")
/*
*/
v[0]=0;
CYCLE(cur, "Firm")
{
	v[0] += VS(cur, "InterestDepositsF");
}

RESULT(v[0] )


EQUATION("SumInterestDepositsEF")
/*
*/

v[0]=0;
CYCLE(cur, "EnergyFirm")
{
	v[0] += VS(cur, "InterestDepositsEF");
}
RESULT(v[0] )

EQUATION("SumInterestDepositsKF")
/*
*/
v[0]=0;
CYCLE(cur, "KFirm")
{
	v[0] += VS(cur, "InterestDepositsKF");
}

RESULT(v[0] )



EQUATION("SumOverdraftPaymentC")
/*
*/

v[0]=0;
CYCLE(cur, "Class")
	v[0] += VS(cur, "OverdraftPaymentC");


RESULT(v[0] )

EQUATION("SumOverdraftPaymentF")
/*
*/
v[0]=0;
CYCLE(cur, "Firm")
{
	v[0] += VS(cur, "OverdraftPaymentF");
}

RESULT(v[0] )

EQUATION("SumOverdraftPaymentEF")
/*
*/
v[0]=0;
CYCLE(cur, "EnergyFirm")
 v[0] += VS(cur, "OverdraftPaymentEF");


RESULT(v[0] )

EQUATION("SumOverdraftPaymentKF")
/*
*/
v[0]=0;
CYCLE(cur, "KFirm")
 v[0] += VS(cur, "OverdraftPaymentKF");

RESULT(v[0] )

EQUATION("ControlODPaymentF")
/*
Comment
*/

#ifdef NOCHECKS
END_EQUATION(-1);
#endif

v[6] = VL("RemovedOverDraftFval",0);

v[0] = VL("SumOverdraftF", 1);
v[1]=0;
CYCLE(cur, "Firm")
{
	v[2] = VLS(cur, "CashF", 1);
	if(v[2]<0)
	 v[1]-=v[2];
}

if(v[0]!=0 && abs(v[0]-v[1]+v[6])/v[0]>0.01)
 INTERACT("WRONG OD", v[0]-v[1]+v[6]);
v[3] = VL("InterestRate", 1);
v[4]=(v[1]-v[6])*v[3];
v[5] = V("SumOverdraftPaymentF");

if(v[4]!=0 && abs(v[4]-v[5])/v[4]>0.01)
 INTERACT("WRONG OD paym", v[4]);

RESULT(1 )

EQUATION("RemovedOverDraftFval")
/*
Comment
*/
V("Exit");
v[6] = V("RemovedOverDraftF");
RESULT(v[6] )


EQUATION("TotalDepositsF")
/*
Deposits FD firms
*/
V("FinancialTrading");
v[0]=0;
CYCLE(cur, "Firm")
{
	v[1] = VS(cur, "CashF");
	if(v[1]>0)
	 v[0]+=v[1];
}
RESULT(v[0] )

EQUATION("TotalDepositsC")
/*
Deposits consumers
*/
V("FinancialTrading");
v[0]=0;
CYCLE(cur, "Class")
{
	v[1] = VS(cur, "CashC");
	if(v[1]>0)
	 v[0]+=v[1];
}
RESULT(v[0] )

EQUATION("TotalDepositsK")
/*
Deposits K firms
*/
V("FinancialTrading");
v[0]=0;
CYCLE(cur, "KFirm")
{
	v[1] = VS(cur, "CashKF");
	if(v[1]>0)
	 v[0]+=v[1];
}
RESULT(v[0] )

EQUATION("TotalDepositsEF")
/*
Deposits energy firms
*/
V("FinancialTrading");
v[0]=0;
CYCLE(cur, "EnergyFirm")
{
	v[1] = VS(cur, "CashEF");
	if(v[1]>0)
	 v[0]+=v[1];
}
RESULT(v[0] )

EQUATION("ChTotalDepositsF")
/*
Change in total deposit F
*/
v[0] = V("TotalDepositsF");
v[1] = VL("TotalDepositsF", 1);
RESULT(v[0]-v[1] )

EQUATION("ChTotalDepositsC")
/*
Change in total deposit class
*/
v[0] = V("TotalDepositsC");
v[1] = VL("TotalDepositsC", 1);
RESULT(v[0]-v[1] )

EQUATION("ChTotalDepositsK")
/*
Change in total deposit K
*/
v[0] = V("TotalDepositsK");
v[1] = VL("TotalDepositsK", 1);
RESULT(v[0]-v[1] )

EQUATION("ChTotalDepositsEF")
/*
Change in total deposit EF
*/
v[0] = V("TotalDepositsEF");
v[1] = VL("TotalDepositsF", 1);
RESULT(v[0]-v[1] )



EQUATION("TotalAssetF")
/*
Total bank assets in FD firms
*/

V("FinancialTrading");

v[0]=0;
CYCLE(cur, "Firm")
{
	v[1] = VS(cur, "CashF");
	if(v[1]<=0)
	 v[0]+=-v[1];
	CYCLES(cur, cur1, "Capital")
	{
		v[0] += VS(cur1, "PrincipalF");
	}
}


RESULT(v[0] )

EQUATION("TotalAssetC")
/*
Total bank assets in Class of consumers
*/

V("FinancialTrading");

v[0]=0;
CYCLE(cur, "Class")
{
	v[1] = VS(cur, "CashC");
	if(v[1]<=0)
	 v[0]+=-v[1];
}

RESULT(v[0] )

EQUATION("TotalAssetK")
/*
Total bank assets in FD firms
*/

V("FinancialTrading");

v[0]=0;
CYCLE(cur, "KFirm")
{
	v[1] = VS(cur, "CashKF");
	if(v[1]<=0)
	 v[0]+=-v[1];
}


RESULT(v[0] )

EQUATION("TotalAssetEF")
/*
Total bank assets in FD firms
*/

V("FinancialTrading");

v[0]=0;
CYCLE(cur, "EnergyFirm")
{
	v[1] = VS(cur, "CashEF");
	if(v[1]<=0)
	 v[0]+=-v[1];
	CYCLES(cur, cur1, "CapitalEF")
	{
		v[0] += VS(cur1, "PrincipalEF");
	}
}


RESULT(v[0] )

EQUATION("ChTotalAssetF")
/*
Change in total asset F
*/
v[0] = V("TotalAssetF");
v[1] = VL("TotalAssetF", 1);
RESULT(v[0]-v[1] )

EQUATION("ChTotalAssetC")
/*
Change in total asset F
*/
v[0] = V("TotalAssetC");
v[1] = VL("TotalAssetC", 1);
RESULT(v[0]-v[1] )

EQUATION("ChTotalAssetK")
/*
Change in total asset F
*/
v[0] = V("TotalAssetK");
v[1] = VL("TotalAssetK", 1);
RESULT(v[0]-v[1] )

EQUATION("ChTotalAssetEF")
/*
Change in total asset F
*/
v[0] = V("TotalAssetEF");
v[1] = VL("TotalAssetEF", 1);
RESULT(v[0]-v[1] )

EQUATION("ChGovernmentFund")
/*
Change in gov. debt
*/
v[0] = V("GovernmentFund");
v[1] = VL("GovernmentFund", 1);
RESULT(v[0]-v[1] )

EQUATION("StatTotalAssets")
/*
Comment
*/
v[0] = V("TotalAssetC");
v[1] = V("TotalAssetF");
v[2] = V("TotalAssetEF");
v[3] = V("TotalAssetK");
v[4]=v[0]+v[1]+v[2]+v[3];
RESULT(v[4] )

EQUATION("StatTotalDeposits")
/*
Comment
*/
v[0] = V("TotalDepositsC");
v[1] = V("TotalDepositsF");
v[2] = V("TotalDepositsEF");
v[3] = V("TotalDepositsK");
v[4]=v[0]+v[1]+v[2]+v[3];
RESULT(v[4] )

EQUATION("ChStatAssets")
/*
Comment
*/
v[0] = V("StatTotalAssets");
v[1] = VL("StatTotalAssets", 1);
RESULT(v[0]-v[1] )

EQUATION("ChStatDeposits")
/*
Comment
*/
v[0] = V("StatTotalDeposits");
v[1] = VL("StatTotalDeposits", 1);
RESULT(v[0]-v[1] )

EQUATION("FinancialTrading")
/*
Collect from household classes and firms (C and K) the relevant financial aggregate
*/
//v[10]=t; //sprintf(msg, "\n FinancialTrading(%g)", v[10]); plog(msg);


V("Production");
v[0]=v[1]=v[2]=v[3]=v[4]=v[5]=v[6]=v[7]=v[8]=v[9]=v[10]=v[11]=v[12]=v[13]=v[14]=v[15]=v[16]=v[17]=v[18]=v[19]=v[20]=v[21]=v[22]=v[23]=v[24]=v[25]=v[26]=v[27]=v[28]=v[29]=v[30]=v[31]=v[32]=v[33]=v[34]=v[35]=v[36]=v[37]=v[38]=v[39]=v[40]=v[41]=v[42]=v[43]=v[44]=v[45]=v[46]=v[47]=v[48]=v[49]=v[50]=v[60]=v[61]=0;


CYCLE(cur, "Class")
 {
  v[6]=VS(cur,"CashC");
  if(v[6]>0)
   v[0]+=v[6];
  else
   v[1]+=-v[6];
  v[8] += VS(cur, "OverdraftPaymentC");
  v[6]=VLS(cur,"CashC",1);
  if(v[6]>0)
   v[60]+=v[6];
  else
   v[61]+=-v[6];

 }

CYCLE(cur, "Firm")
 {
   v[16]=VS(cur,"CashF");
  if(v[16]>0)
   v[10]+=v[16];
  else
   v[11]+=-v[16];
  v[14] += VS(cur, "OutstandingDebtF");
  v[15] += VS(cur, "CapitalPaymentF");
  v[19] += VS(cur, "InterestPaymentF");
  v[18] += VS(cur, "OverdraftPaymentF");
  v[6]=VLS(cur,"CashF",1);
  if(v[6]>0)
   v[60]+=v[6];
  else
   v[61]+=-v[6];

 }

CYCLE(cur, "KFirm")
 {
  v[26]=VS(cur,"CashKF");
  if(v[26]>0)
   v[20]+=v[26];
  else
   v[21]+=-v[26];
  v[28] += VS(cur, "OverdraftPaymentKF");
  v[6]=VLS(cur,"CashKF",1);
  if(v[6]>0)
   v[60]+=v[6];
  else
   v[61]+=-v[6];

 }

cur = SEARCH("Energy");

CYCLES(cur, cur1, "EnergyFirm")
{
  v[36]=VS(cur1,"CashEF");
  if(v[36]>0)
   v[30]+=v[36];
  else
   v[31]+=-v[36];
  v[34] += VS(cur1, "OutstandingDebtEF");
  v[35] += VS(cur1, "CapitalPaymentEF");
  v[39] += VS(cur1, "InterestPaymentEF");
  v[38] += VS(cur1, "OverdraftPaymentEF");
  v[6]=VLS(cur,"CashEF",1);
  if(v[6]>0)
   v[60]+=v[6];
  else
   v[61]+=-v[6];

}

v[74]=V("GovernmentFund");

v[40]=v[0]+v[10]+v[20]+v[30]; //deposits
v[41]=v[1]+v[11]+v[21]+v[31]; //overdraft
v[44]=v[14]+v[34] - v[74]; // EF and F OutstandingDebt
v[45]=v[15]+v[35]; // CapitalPayment EF and F (sum of principle repayments)
v[48]=v[8]+v[18]+v[28]+v[38]; //overdraft payments (interest payments on overdrafts)
v[49]=v[19]+v[39]; // interest payment on loans
v[50]=v[48]+v[49]; // total interest payments on loans and overdrafts

RESULT(1)


EQUATION("TotLiabilitiesF")
/*
Liabilities F
*/
v[1] = V("SumOverdraftF");
v[0] = V("TotalOutstandingDebtF");
RESULT(v[0]+v[1] )

EQUATION("TotLiabilitiesEF")
/*
Liabilities EF
*/
v[1] = V("SumOverdraftEF");
v[0] = V("TotalOutstandingDebtEF");
RESULT(v[0]+v[1] )

EQUATION("TotLiabilitiesHH")
/*
Liabilities EF
*/
v[1] = V("SumOverdraftC");
RESULT(v[1] )

EQUATION("TotLiabilitiesKF")
/*
Liabilities EF
*/
v[1] = V("SumOverdraftK");
RESULT(v[1] )


EQUATION("TotPCLiabilitiesHH")
/*
Share of liabilities for consumers
*/

v[1] = V("SumOverdraftC");
v[2] = V("TotalOutstandingDebt");
v[3] = V("TotalOverdraft");

v[4]=v[1]/(v[2]+v[3]);
RESULT(v[4] )

EQUATION("TotPCLiabilitiesF")
/*
Share of liabilities for FD firms
*/

v[1] = V("SumOverdraftF");
v[0] = V("TotalOutstandingDebtF");
v[2] = V("TotalOutstandingDebt");
v[3] = V("TotalOverdraft");

v[4]=(v[0]+v[1])/(v[2]+v[3]);
RESULT(v[4] )

EQUATION("TotPCLiabilitiesEF")
/*
Share of liabilities for energy firms
*/

v[1] = V("SumOverdraftEF");
v[0] = V("TotalOutstandingDebtEF");
v[2] = V("TotalOutstandingDebt");
v[3] = V("TotalOverdraft");

v[4]=(v[0]+v[1])/(v[2]+v[3]);
RESULT(v[4] )

EQUATION("TotPCLiabilitiesKF")
/*
Share of liabilities for K firms
*/

v[1] = V("SumOverdraftK");
v[2] = V("TotalOutstandingDebt");
v[3] = V("TotalOverdraft");

v[4]=v[1]/(v[2]+v[3]);
RESULT(v[4] )

EQUATION("TotalInterestCollected")
/*
Total revenues for the bank, made by payment on existing loans of any type
*/

v[0] = V("TotalInterestPaymentF");
v[1] = V("SumOverdraftPaymentC");
v[2] = V("SumOverdraftPaymentKF");
v[3] = V("TotalInterestPaymentEF");
//v[5] = V("InterestGovFund");
v[4]=v[0]+v[1]+v[2]+v[3];
RESULT(v[4] )

EQUATION("TotalInterestPaymentF")
/*
Payments to the bank
*/
v[0] = V("SumOverdraftPaymentF");
v[1] = V("SumInterestPaymentF");

v[2]=v[0]+v[1];
RESULT(v[2] )

EQUATION("TotalInterestPaymentEF")
/*
Payments to the bank by energy firms
*/
v[0] = V("SumOverdraftPaymentEF");
v[1] = V("SumInterestPaymentEF");

v[2]=v[0]+v[1];
RESULT(v[2] )

EQUATION("TotalOutstandingDebt")
/*
Outstanding debt for K purchase by F and EF, plus public debt
*/

v[0] = V("TotalOutstandingDebtF");
v[1] = V("TotalOutstandingDebtEF");
v[2] = V("GovernmentFund");
v[3]=v[0]+v[1]-v[2];
RESULT(v[3] )

EQUATION("TotalOutstandingDebtF")
/*
Outstanding debt for FD firms
*/

v[0]=0;

CYCLE(cur, "Firm")
	v[0] += VS(cur, "OutstandingDebtF");

RESULT(v[0] )

EQUATION("TotalOutstandingDebtEF")
/*
Outstanding debt for FD firms
*/

v[0]=0;

CYCLE(cur, "EnergyFirm")
 v[0] += VS(cur, "OutstandingDebtEF");

RESULT(v[0] )


EQUATION("TotalOverdraft")
/*
Sum of overdraft
*/
v[0] = V("SumOverdraftF");
v[1] = V("SumOverdraftC");
v[2] = V("SumOverdraftK");
v[3] = V("SumOverdraftEF");
v[4]=v[1]+v[2]+v[3]+v[0];
RESULT(v[4] )

EQUATION("SumOverdraftF")
/*
Overdraft for FD firms
*/
v[0]=0;
V("Exit");
CYCLE(cur, "Firm")
{
	v[1] = VS(cur, "CashF");
	if(v[1]<0)
	 v[0]-=v[1];
}

RESULT(v[0] )

EQUATION("SumOverdraftC")
/*
Overdraft for consumer classes
*/
v[0]=0;

CYCLE(cur, "Class")
{
	v[1] = VS(cur, "CashC");
	if(v[1]<0)
	 v[0]-=v[1];
}

RESULT(v[0] )

EQUATION("SumOverdraftK")
/*
Overdraft for K firms
*/
v[0]=0;

CYCLE(cur, "KFirm")
{
	v[1] = VS(cur, "CashKF");
	if(v[1]<0)
	 v[0]-=v[1];
}

RESULT(v[0] )

EQUATION("SumOverdraftEF")
/*
Overdraft for energy firms
*/
v[0]=0;

CYCLE(cur, "EnergyFirm")
{
	v[1] = VS(cur, "CashEF");
	if(v[1]<0)
	 v[0]-=v[1];
}

RESULT(v[0] )

EQUATION("BankCapital")
/*
Bank capital buffer increased by interest rate differences and reduced by non-returned loans
*/

V("FinancialTrading");

if(t==1)
 {
  v[71] = V("TotalAssets");
  v[70] = V("StatTotalDeposits");
  v[72]=v[71]-v[70];
  //LOG("\n%lf\t%lf\t%lf", v[70],v[71],v[72]);
  END_EQUATION(v[72]);
 }

v[0] = VL("BankCapital", 1);
if(v[0]<0)
 v[0]=0;//gov rescued the bank 
v[1] = V("TotalInterestCollected");
v[2] = V("SumInterestDepositsF");
v[3] = V("SumInterestDepositsC");
v[4] = V("SumInterestDepositsKF");
v[5] = V("SumInterestDepositsEF");
v[6] = VL("ExitEF",1);
v[7] = VL("Exit",1);
v[50] = V("SumCashInjectionKF");
v[30] = VL("InterestGovFund",1);
v[9] = VL("DistributedProfitsBank",1);
v[88]=v[8]=v[0]+v[1]+v[30]-v[2]-v[3]-v[4]-v[5]-v[6]-v[7]-v[9]-v[50];

WRITE("Adjustment", 0);
WRITE("RelAdj", 0);

if(v[8]<0)
 {//HAIRCUT, reduction of positive bank accounts to finance the loss of central bank
//  WRITE("Gov2Bank", -v[8]);
  //INTERACT("Neg. BankCapital", v[8]);
 }
END_EQUATION(v[8]);
/****************************/

if(v[8]<0)
 {//HAIRCUT, reduction of positive bank accounts to finance the loss of central bank
  INTERACT("Neg. BankCapital", v[8]);
  WRITE("Adjustment", v[8]);
  v[20]=v[21]=v[22]=v[23]=v[24]=0;
  v[9] = V("StatTotalDeposits");

if(v[9]==0)
	INTERACT("Zero TotalDeposits",v[9]);


  v[10]= 1+v[8]/v[9];
  WRITE("RelAdj", v[10]);

  if(v[10]<0)
   INTERACT("Neg. tot assets",v[9]-v[8]);
  v[11]=0;
  cur2 = SEARCHS(p->up, "Supply");
  CYCLES(cur2, cur, "Firm")
  {v[20]=1;
   cur1 = SEARCHS(cur, "BankF");
   v[25]=VS(cur1, "CashF");
   if(v[25]>0)
   {
   v[24] += v[25];
   v[11] += MULTS(cur1, "CashF", v[10]);
   }
  }

  cur2 = SEARCHS(p->up, "Demand");
  CYCLES(cur2, cur, "Class")
  {v[21]=1;

   v[25]=VS(cur, "CashC");
   if(v[25]>0)
   {
   v[24] += v[25];
   v[11] += MULTS(cur, "CashC", v[10]);
   }
  }

  cur2 = SEARCHS(p->up, "Machinery");
  CYCLES(cur2, cur, "KFirm")
  {v[22]=1;
   cur1 = SEARCHS(cur, "BankK");

   v[25]=VS(cur1, "CashKF");
   if(v[25]>0)
   {
   v[24] += v[25];
   v[11] += MULTS(cur1, "CashKF", v[10]);
   }
  }

  cur2 = SEARCHS(p->up, "Energy");
  CYCLES(cur2, cur, "EnergyFirm")
  {
   v[23]=1;

   v[25]=VS(cur, "CashEF");
   if(v[25]>0)
   {
   v[24] += VS(cur, "CashEF");
   v[11] += MULTS(cur, "CashEF", v[10]);

   //sprintf(msg, "\n CashEF(%g)", v[24]); plog(msg);
   //sprintf(msg, "\n v10(%g)", v[10]); plog(msg);

   //LOG("\nHaircut: CashEF %lf. Haircut: %lf \n\n)",v[24], v[10]);
   }
  }

 //LOG("\nHaircut: bankK %lf. Haircut: %lf \n\n)",v[8], v[10]);

 if(abs(v[9]+v[8]-v[11])/v[11]>0.1)
  INTERACT("\nWrong haircut %lf\n", (v[9]+v[8]-v[11])/v[11]);
 INCR("TotalDeposits", v[8]);
 v[8]=0;
 }


v[31] = V("TotalAssets");
v[37] = V("ProfitShareTargetBank"); // parameter
v[38] = V("sGDPnom");


if(v[8]>0)
  v[35]=min(v[8],v[38]*v[37]);
 else
  v[35]=0;



if(v[35]>0 && 2==3)
 {//SOCIAL DIVIDEND, distribution to HH of excessive BankCapital
  //v[35]=v[37] * v[38]; //Capital to distribute
  v[32]=0;
  WRITE("Adjustment", v[35]);
  cur2 = SEARCHS(p->up, "Demand");
  CYCLES(cur2, cur, "Class")
  	v[32]+=VS(cur, "CashC");

  if(v[32]==0)
  	INTERACTS(cur2, "Zero CashC",v[32]);

  v[33]=1+v[35]/v[32];
  WRITE("RelAdj", v[33]);

  v[34]=0;
  CYCLES(cur2, cur, "Class")
   v[34]+=MULTS(cur, "CashC",v[33]);

//  LOG("\nSocial Dividend: percentage increment %lf.  BankK/GDP %lf. BankK/Assets %lf\n\n)",v[35]/v[32], (v[8]-v[35])/v[38], v[8]/v[31]);
  if(abs((v[34]-v[32]-v[35])/v[34])>0.01)
   INTERACT("Error in Social Dividend", (v[34]-v[32]-v[35])/v[34]);

  v[8]-=v[35];
  INCR("TotalDeposits", v[35]);

 }


v[80] = V("GDPnom");
v[81] = V("Adjustment");
WRITE("RatioAdjGDPm", v[81]/v[80]);

/*
//Control code to check the correct computation of the interest flow. To be used for debugging purposes

v[11]=v[12]=v[13]=v[14]=v[15]=0;

CYCLES(p->up, cur, "Firm")
{
 v[12] += VS(cur, "InterestDepositsF");
 v[11] += VS(cur, "InterestPaymentF");
 v[11] += VS(cur, "OverdraftPaymentF");
}

CYCLES(p->up, cur, "KFirm")
{
 v[14] += VS(cur, "InterestDepositsKF");
 v[11] += VS(cur, "OverdraftPaymentKF");
}

CYCLES(p->up, cur, "Class")
{
 v[13] += VS(cur, "InterestDepositsC");
 v[11] += VS(cur, "OverdraftPaymentC");
}

CYCLES(p->up, cur, "EnergyFirm")
{
 v[15] += VS(cur, "InterestDepositsEF");
 v[11] += VS(cur, "InterestPaymentEF");
 v[11] += VS(cur, "OverdraftPaymentEF");
}

PLOG("\nFirm:\nTotal %lf\nCompu %lf\nDiffe %lf\n",v[2],v[12],v[2]-v[12]);
PLOG("\nConsumer:\nTotal %lf\nCompu %lf\nDiffe %lf\n",v[3],v[13],v[3]-v[13]);
PLOG("\nKapital Firm:\nTotal %lf\nCompu %lf\nDiffe %lf\n",v[4],v[14],v[4]-v[14]);
PLOG("\nEnergy Firm:\nTotal %lf\nCompu %lf\nDiffe %lf\n",v[5],v[15],v[5]-v[15]);

PLOG("\nCollected:\nTotal %lf\nCompu %lf\nDiffe %lf\n",v[1],v[11],v[1]-v[11]);

v[10]=v[0]+v[11]-v[12]-v[13]-v[14]-v[15]-v[6]-v[7];
*/
RESULT(v[8] )


EQUATION("InterestGovFund")
/*
Interest paid on gov. debt
*/
v[0] = V("GovernmentFund");
v[3] = V("shareInterestGov");
v[1] = V("InterestRate")*v[3];
v[2]=-v[1]*v[0];
RESULT(v[2] )

EQUATION("GovDebtGDPRatio")
/*
Ratio of debt to GDP
*/
v[0] = V("GDPnom");
v[1] = V("GovernmentFund");
v[2] = -v[1]/v[0];
RESULT(v[2] )

EQUATION("GovernmentFund")
/*
Money available to the government
*/
v[0] = VL("GovernmentFund",1);
v[1] = V("SumTaxF");
v[2] = V("SumTaxK");
v[3] = V("SumTaxEF");
v[4] = V("GovernmentExpenses");
v[6] = VL("InterestGovFund",1);
v[7] = VL("IncomeTaxTot",0);
v[8] = VL("GovDemTot",1);
v[11] = V("ExtraGovSpending");
v[9] = VL("BankCapital",1);
if(v[9]<0)
 v[10]=v[9];
else
 v[10]=0; 
v[5]=v[0]+v[1]+v[2]+v[3]-v[4]-v[6]-v[8]-v[11]+v[7]+v[10];
RESULT(v[5] )

EQUATION("GovDeficit")
/*
Government deficit
*/
v[0] = V("GovernmentFund");
v[1] = VL("GovernmentFund", 1);
v[2]=v[0]-v[1];
RESULT( v[2])



EQUATION("GovernmentExpenses")
/*
Gov. expenses
*/
v[0] = V("GDP");
v[1] = V("minShareGovGDP");
v[2] = V("maxShareGovGDP");
v[3] = V("UnemploymentRate");


v[4]=v[1]+(v[3]-0.05)*(v[2]-v[1]);
v[5]=max(0,v[4]);
WRITE("shareGovGDP", v[5]);


RESULT(1000000)

EQUATION("Subsidies")
/*
Gov. subsidies
*/
v[0] = VL("GovernmentExpenses",1);
v[1] = V("Individuals");
v[2] = V("TotIndividuals");
v[3]=v[0]*v[1]/v[2];
RESULT(v[3] )

EQUATION("Income")
/*
Comment
*/
V("Production");
v[0]=V("PremiaIncome");
v[1]=V("WageIncome");
v[14]=V("InterestDepositsC");/**/

v[16]=V("OverdraftPaymentC");/**/
v[20]=V("ExcessCostIncome");


v[3] = VL("Subsidies",1);
v[6] = V("IncomeTax");
v[7] = VL("BankProfitIncome",1);

v[5]=v[0]+v[1]+v[14]+v[3]+v[7]-v[16]+v[20]-v[6];

RESULT(v[5])

EQUATION("IncomeTax")
/*
Income tax
*/

v[0]=V("PremiaIncome");
v[1]=V("WageIncome");
v[14]=V("InterestDepositsC");/**/

v[16]=V("OverdraftPaymentC");/**/
v[20]=V("ExcessCostIncome");


v[2]=v[0]+v[1]+v[14]-v[16]+v[20];

v[1] = V("SavingRate");
v[4] = V("IncomeTaxShare");
v[3]=v[2]*v[1]*v[4];
RESULT(v[3] )

EQUATION("IncomeTaxTot")
/*
Sum of total income tax
*/

v[0]=0;
CYCLE(cur, "Class")
{
	v[0] += VS(cur, "IncomeTax");
}

RESULT(v[0] )

EQUATION("Profit")
/*
Profit, difference between revenues, total costs and RD
*/
//v[0]=V("Revenues");
v[0]=V("MonetarySales"); //flo

v[2]=V("LaborCost");
v[3]=V("RdExpenditure");
v[4]=V("ENCost");
//v[6]=V("CapitalCostF");
v[7]=V("OverdraftPaymentF");/**/
v[8]=V("InterestDepositsF");/**/

v[9]=V("CapitalPaymentF");
v[10]=V("InterestPaymentF");/**/

/*
if(v[10]==0 || v[9]==0)
 {
  v[11]=0;
  CYCLE(cur, "Capital")
  {
  	v[11] += VS(cur, "PrincipalF");
  }
  //if(abs(v[11])>0.000001)
   //INTERACT("Merde-profit", v[11]);
 }
*/

v[5]=v[0]-v[2]-v[3]-v[4]-v[7]+v[8]-v[9]-v[10];
RESULT(v[5] )



EQUATION("KProfit")
/*
Comment
*/
V("KNbrEngineers"); // compute first the number of engineers which use the past value of cumulated profits
V("KProductionFlow");
V("SumKOandMRevenue");
//v[0]=V("KPrice");
v[0]=V("KRevenues");
v[2]=V("KENCost");
v[3]=V("KLaborCost");
v[4]=V("OverdraftPaymentKF");/**/
v[8]=V("InterestDepositsKF");/**/
v[9]=V("KOandMRevenue");

v[5]=v[0]-v[2]-v[3]-v[4]+v[8]+v[9];
v[6]=INCR("KCumProfit",v[5]);
RESULT(v[5] )


EQUATION("TaxRevenues")
/*
Global tax revenues
*/

v[0] = V("SumTaxF");
v[1] = V("SumTaxEF");
v[2] = V("SumTaxK");

RESULT(v[0]+v[1]+v[2] )

EQUATION("TaxF")
/*
Tax paid on profit of F firm
*/

v[0] = V("taxrate");
v[1] = V("Profit");
if(v[1]>0)
 v[2]=v[0]*v[1];
else
 v[2]=0;
RESULT(v[2] )

EQUATION("TaxK")
/*
Tax paid on profit of F firm
*/

v[0] = V("taxrate");
v[1] = V("KProfit");
if(v[1]>0)
 v[2]=v[0]*v[1];
else
 v[2]=0;
RESULT(v[2] )

EQUATION("TaxEF")
/*
Tax paid on profit of F firm
*/

v[0] = V("taxrate");
v[1] = V("ProfitEF");
if(v[1]>0)
 v[2]=v[0]*v[1];
else
 v[2]=0;
RESULT(v[2] )


EQUATION("SumTaxF")
/*
Total tax Firm
*/

v[0]=0;
CYCLE(cur, "Firm")
{
	v[0] += VS(cur, "TaxF");
}

RESULT(v[0] )


EQUATION("SumTaxK")
/*
Total tax KFirm
*/

v[0]=0;
CYCLE(cur, "KFirm")
{
	v[0] += VS(cur, "TaxK");
}

RESULT(v[0] )

EQUATION("SumTaxEF")
/*
Total tax EFFirm
*/
v[0]=0;
CYCLE(cur, "EnergyFirm")
{
	v[0] += VS(cur, "TaxEF");
}

RESULT(v[0] )

EQUATION("BalanceC")
/*
Comment
*/
v[1]=V("Income");
v[2]=V("Expenditure");
v[3]=v[1]-v[2];
//if(v[3]<0)
 //INTERACT("mEG BalC.",v[3]);
RESULT(v[3] )

EQUATION("BalanceF")
/*
Balance of the current account for firms
*/
v[0]=V("Profit");
v[1]=V("WagePrem");
v[2] = V("TaxF");

v[3]=v[0]-v[1]-v[2];
RESULT(v[3])

EQUATION("BalanceK")
/*
Balance of the current account for firms
*/
v[0]=V("KProfit");
v[1]=V("KWagePrem");
v[2] = V("TaxK");

v[3]=v[0]-v[1]-v[2];
RESULT(v[3])

EQUATION("BalanceEF")
/*
Cash revenues for energy firms
*/
V("PriceEN");
v[0] = V("ProfitEF");
v[1] = V("WagePremEF");
v[2] = V("TaxEF");

v[3]=v[0]-v[1]-v[2];
RESULT(v[3])


EQUATION("CashC")
/*
Cash in the account of consumers
*/
v[0] = VL("CashC", 1);
v[1] = V("BalanceC");
v[2]=v[0]+v[1];
WRITE("OrganicCashC", v[2]);
if(v[2]<0)
 INTERACT("mEG c.",v[1]);
RESULT(v[2] )

EQUATION("CashF")
/*
Cash in deposits for F
*/
v[0] = VL("CashF", 1);
v[1] = V("BalanceF");

v[3]=v[0]+v[1];
WRITE("OrganicCashF", v[3]);
RESULT(v[3])

EQUATION("CashFAdjustment")
/*
Variation of CashC due to Bank's actions
*/

V("BankCapital");
v[0] = V("CashF");
v[1] = V("OrganicCashF");
v[2]=v[1]-v[0];
RESULT(v[2] )

EQUATION("CashKF")
/*
Cash in bank deposit
*/
v[0] = VL("CashKF", 1);
v[1] = V("BalanceK");
v[2] = V("CashInjectionKF");

v[3]=v[0]+v[1]+v[2];
WRITE("OrganicCashKF", v[3]);
RESULT(v[3] )

EQUATION("CashKFAdjustment")
/*
Variation of CashKF due to Bank's actions
*/

V("BankCapital");
v[0] = V("CashKF");
v[1] = V("OrganicCashKF");
v[2]=v[1]-v[0];
RESULT(v[2] )

EQUATION("CashEF")
/*
Comulative cash of energy firms
*/
V("PriceEN");
v[0] = VL("CashEF", 1);
v[1] = V("BalanceEF");

v[3]=v[0]+v[1];
WRITE("OrganicCashEF", v[3]);
//V("DemographyEN");
v[4] = V("PPremoved");
v[5]=v[3]-v[4];
RESULT(v[5])

EQUATION("CashEFAdjustment")
/*
CashEF correction to the Bank's action
*/
//V("DemographyEN"); // make sure CashEF is updated if a PP is removed

V("BankCapital");
v[0] = V("CashEF");
v[1] = V("OrganicCashEF");
v[2]=v[1]-v[0];
RESULT(v[2] )

EQUATION("CashInjectionKF")
/*
Remove debt for KF with too poor Health
*/
v[22]=0;

v[19]=VL("minRRoKExit",1);
v[20]=VL("RRoKKF",1);
v[21]=VL("CashKF",1);

if(v[19]>v[20] && v[21]<0)
 {
 v[22]= -v[21];
 }

RESULT(v[22])


EQUATION("SumCashInjectionKF")
/*
Remove debt for KF with too poor Health
*/
v[0]=0;
CYCLE(cur,"KFirm")
{
	v[0]+=VS(cur,"CashInjectionKF");
}
RESULT(v[0] )


EQUATION("CapitalPaymentF")
/*
Cost of repayment principal
*/
v[0]=0;
CYCLE(cur, "Capital")
{
	v[0]+=VS(cur, "InstallmentF");
}
RESULT(v[0] )

EQUATION("CapitalPaymentEF")
/*
Cost of repayment principal
*/
v[0]=0;
CYCLE(cur, "CapitalEF")
{
	v[0]+=VS(cur, "InstallmentEF");
}
RESULT(v[0] )




EQUATION("InstallmentF")
/*
Share of loans returned
*/
v[0] = VL("PrincipalF", 1);
if(v[0]<=0)
 END_EQUATION(0);
v[2] = V("KExpenditures");
v[1] = V("TimeRepayment");

v[4]=min(v[0],v[2]/v[1]);
RESULT(v[4] )

EQUATION("InstallmentEF")
/*
Remaining loan to be returned
*/
v[0] = VL("PrincipalEF", 1);
if(v[0]<=0)
 END_EQUATION(0);
v[2] = V("KExpendituresEN");
v[1] = V("TimeRepaymentEF");

v[4]=min(v[0],v[2]/v[1]);
RESULT(v[4] )




EQUATION("PrincipalF")
/*
Remaining loan to be returned
*/
v[0] = VL("PrincipalF", 1);
v[1] = V("InstallmentF");

RESULT(v[0]-v[1] )

EQUATION("PrincipalEF")
/*
Remaining loan to be returned
*/
v[0] = VL("PrincipalEF", 1);
v[1] = V("InstallmentEF");

RESULT(v[0]-v[1] )




EQUATION("InterestF")
/*
Total interest to pay on the loan
*/
v[0] = VL("PrincipalF",1);
v[1] = VL("InterestRate",1);

RESULT(v[0]*v[1] )

EQUATION("InterestEF")
/*
Total interest to pay on the loan
*/
v[0] = VL("PrincipalEF",1);
v[1] = VL("InterestRate",1);

RESULT(v[0]*v[1] )




EQUATION("InterestPaymentF")
/*
Interest payments on outstanding debt
*/
v[0]=0;
CYCLE(cur, "Capital")
{
	v[0]+=VS(cur, "InterestF");
}

RESULT(v[0] )

EQUATION("InterestPaymentEF")
/*
Interest payments on outstanding debt
*/
//V("DemographyEN");
v[0]=0;
CYCLE(cur, "CapitalEF")
{
	v[0]+=VS(cur, "InterestEF");
}

RESULT(v[0] )





EQUATION("OutstandingDebtFXXX")
/*
Stock of existing debt
*/
v[0]=0;
CYCLE(cur, "Capital")
{
	v[0]+=VS(cur, "PrincipalF");
}

RESULT(v[0] )

EQUATION("OutstandingDebtF")
/*
Stock of existing debt
*/
v[0] = VL("OutstandingDebtF",1);
V("InvestmentDecision");
v[1] = V("InvestmentSpending");
v[2] = V("CapitalPaymentF");
v[3] = v[0]+v[1]-v[2];

v[10]=0;
CYCLE(cur, "Capital")
{
	v[10]+=VS(cur, "PrincipalF");
}
v[11]=max(abs(v[3]),abs(v[10]));
if(v[11]>10 && abs(v[10]-v[3])/v[11]>0.000001)
 INTERACT("Outstanding debt mismatch", v[3]-v[10]);
RESULT(v[3] )

EQUATION("OutstandingDebtEF")
/*
Stock of existing debt
*/
v[0]=0;
V("InvestmentDecisionEF");
CYCLE(cur, "CapitalEF")
{
	v[0]+=VS(cur, "PrincipalEF");
}

RESULT(v[0] )



EQUATION("CapitalCostF")
/*
Cost of repayment loans
*/
v[0]=V("CapitalPaymentF");
v[1]=V("InterestPaymentF");/**/

RESULT(v[0]+v[1] )

EQUATION("CapitalCostEF")
/*
Cost of repayment loans
*/
v[0]=V("CapitalPaymentEF");
v[1]=V("InterestPaymentEF");/**/

RESULT(v[0]+v[1] )




EQUATION("OverdraftPaymentC")
/*
Interest paid on overdraft HH
*/
v[0] = VL("CashC",1);/**/
if(v[0]>0)
 END_EQUATION(0);
v[1] = VL("InterestRate",1);

v[2]=-v[0]*v[1];
RESULT(v[2] )

EQUATION("OverdraftPaymentF")
/*
Interest paid on overdraft FDF
*/
v[0] = VL("CashF",1);/**/
if(v[0]>0)
 END_EQUATION(0);
v[1] = VL("InterestRate",1);

v[2]=-v[0]*v[1];
RESULT(v[2] )

EQUATION("OverdraftPaymentKF")
/*
Interest paid on overdraft KF
*/
v[0] = VL("CashKF",1);/**/
if(v[0]>0)
 END_EQUATION(0);
v[1] = VL("InterestRate",1);

v[2]=-v[0]*v[1];
RESULT(v[2] )

EQUATION("OverdraftPaymentEF")
/*
Interest paid on overdraft EF
*/
v[0] = VL("CashEF",1); /**/
if(v[0]>0)
 END_EQUATION(0);
v[1] = VL("InterestRate",1);

v[2]=-v[0]*v[1];
RESULT(v[2] )



EQUATION("InterestDepositsC")
/*
Cash obtained as interest on deposits
*/
v[0] = V("InterestRateDeposits");
v[1] = VL("CashC",1);/**/
if(v[1]>0)
 v[3]=v[0]*v[1];
else
 v[3]=0;
RESULT(v[3] )

EQUATION("InterestDepositsF")
/*
Cash obtained as interest on deposits
*/
v[0] = V("InterestRateDeposits");
v[1] = VL("CashF",1);/**/
if(v[1]>0)
 v[3]=v[0]*v[1];
else
 v[3]=0;
RESULT(v[3] )

EQUATION("InterestDepositsKF")
/*
Cash obtained as interest on deposits
*/
v[0] = V("InterestRateDeposits");
v[1] = VL("CashKF",1);/**/
if(v[1]>0)
 v[3]=v[0]*v[1];
else
 v[3]=0;
RESULT(v[3] )

EQUATION("InterestDepositsEF")
/*
Cash obtained as interest on deposits
*/
v[0] = V("InterestRateDeposits");
v[1] = VL("CashEF",1);/**/
if(v[1]>0)
 v[3]=v[0]*v[1];
else
 v[3]=0;
RESULT(v[3] )


EQUATION("SumEFirmControlCapital")
/*
*/
RESULT(SUM("EFirmControlCapital") )


EQUATION("SumMaxEnergyCapacity")
/*
*/
v[0]=0;
CYCLE(cur, "EnergyFirm")
{
	v[0] += VS(cur, "MaxEnergyCapacity");
}

RESULT(v[0] )


EQUATION("SumCashEFCh")
/*
*/
V("Trade");
V("FinancialTrading");
V("InvestmentDecision");
V("Production");
RESULT(SUM("CashEFCh") )

EQUATION("Control")
/*
Control the allocation of production
*/

v[0]=V("Q");
v[1]=V("UnitDemand");
v[2]=V("Stocks");
v[3]=VL("Stocks",1);
v[4]=V("backlog");
v[5]=VL("backlog",1);

v[6]=v[0]-v[1]-(v[2]-v[3])+(v[4]-v[5]);
//if(abs(v[6]-v[0])>0.001)
 //INTERACT("Control",v[6]);
RESULT( v[6])


EQUATION("SumWindfall")
/*
Sum of windfall
*/
v[0]=0;
CYCLE(cur, "Firm")
{
	v[0] += VS(cur, "Windfall");
}
v[1] = V("SumRevenues");
if(v[1]>0)
 WRITE("RatioWindfall", v[0]/v[1]);
RESULT(v[0] )

EQUATION("Windfall")
/*
Compute the extra revenues due to extra price
*/
v[0] = V("ExtraPrice");
v[1] = V("UnitDemand");
v[2]=v[1]*v[0];
RESULT(v[2] )

EQUATION("ExtraPrice")
/*
Compute the extra price to be added to standard price when monetary sales exceeds supply
*/
v[0] = V("Q");
v[1] = VL("Stocks", 1);
v[2] = V("MonetarySales");
v[3] = VL("price", 1);
v[4]=v[2]/(v[0]+v[1]);
v[5]=max(v[4]-v[3],0);

RESULT(v[5] )


EQUATION("UnitDemand")
/*
Number of product units demanded
*/
V("Trade"); //ensure that Monetary sales is updated
v[0]=VL("price",1);
v[2] = V("ExtraPrice");
v[1]=V("MonetarySales");

if(v[0]==0)
	END_EQUATION(0);

v[3]=v[1]/(v[0]+v[2]);

RESULT(v[3] )


EQUATION("UnitSales")
/*
Actual supply
*/

v[1]=V("Q");
v[2]=VL("Stocks",1);

v[3]=V("UnitDemand");
v[4]=VL("backlog",1);

v[5]=v[1]+v[2];//capacity
v[6]=v[3]+v[4];//demand

v[0]=min(v[5],v[6]); //UNRECOGNISE GRAMMAR ERROR

if(v[0]<0)
v[0]=0; //UNRECOGNISE GRAMMAR ERROR

//INTERACT("NegSales", v[7]);
RESULT(v[0] )



EQUATION("Stocks")
/*
Stocks
*/
v[1]=VL("Stocks",1);
v[2]=V("Q");
v[3]=V("UnitSales");

v[0]=v[1]+v[2]-v[3];

if(v[0]<0)
v[0]=0;
// INTERACT("NEG. Stocks",v[3]);
RESULT(v[0] )



EQUATION("backlog")
/*
*/
v[1]=V("Q");
v[2]=VL("Stocks",1);

v[3]=V("UnitDemand");
v[4]=VL("backlog",1);

v[5]=v[1]+v[2];//capacity

v[6]=v[3]-v[5];//change in backlogs

v[0]=v[4]+v[6];

if(v[0]<0)
v[0]=0;

RESULT(v[0] )



EQUATION("Revenues")
/*
Money flowing in
*/
v[10]=t;
//sprintf(msg, "\n Revenues(%g)", v[10]); plog(msg);
V("Trade");
v[5]=V("MonetarySales");
V("UnitDemand");
V("backlog");
V("Stocks");

v[0]=V("UnitDemand");
v[1]=VL("price",1)+V("ExtraPrice");
//v[2]=V("backlogSales");
v[2]=V("backlogValueCh"); //we assume that backlog are orders paid by households, but not fulfilled xxx
v[3]=(v[0]*v[1]);//+v[2];
RESULT(v[3] )



EQUATION("NetWorth")
/*
Measure of the value of the firm in case of sales
*/

v[0]=V("KPresentValue")*V("PrudenceLending");
v[1]=V("CashF");
v[2]=VL("OutstandingDebtF",1);//xxx
v[4]=V("backlog");//xxx
//v[4]=V("BacklogValue");//xxx
v[5]=VL("price",1);
v[6]=VL("Stocks",1);


//v[3]=v[0]-v[4]-v[2]+v[1];
v[3]=v[0]-(v[4]*v[5])+(v[6]*v[5])-v[2]+v[1];
RESULT(v[3] )



EQUATION("BacklogValue")
/*
Value of the backlog, future revenues
*/

v[0]=0;
CYCLES(p->up, cur, "blItem")
 {
  v[0]+=VS(cur,"blQ")*VS(cur,"blPrice");
 }

RESULT(v[0] )


EQUATION("BacklogValueCh")
/*
*/
v[0]=V("BacklogValue");
v[1]=VL("BacklogValue",1);
RESULT(v[0]-v[1] )


EQUATION("backlogValueCh")
/*
*/
v[0]=V("backlogValue");
v[1]=V("backlogValueL");
RESULT(v[0]-v[1] )





EQUATION("test")
/*
*/
v[0]=V("SumMonetarySales");
v[1]=V("SumExpenditures");
//v[2]=V("SumBacklogSales");
v[10]=v[0]-v[1];
RESULT(v[10] )


EQUATION("SumExpenditure")
/*
*/
V("Trade");
v[0]=SUM("Expenditure");

RESULT(v[0] )

EQUATION("SumMonetarySales")
/*
*/
V("Trade");
v[0]=SUM("MonetarySales");
RESULT(v[0] )

EQUATION("probEntryEF")
/*
*/
v[0] = V("SumEnergyProductionEF");
v[1] = V("TotENCapacity");
v[2] = V("expPREF");
v[3] = pow(v[0]/v[1],v[2]);

v[4] = V("NbrEF");

v[8] = V("MaxEFirms");
v[9] = V("MinEFirms");
v[6]=v[3];
if(v[4]>v[8])
 v[6]=0;
if(v[4]<v[9])
 v[6]=1;

RESULT(v[6])



EQUATION("ControlShareENPP")
/*
*/
//v[10]=t;sprintf(msg, "\n ShareENPP(%g)", v[10]); plog(msg);

#ifdef NOCHECKS
END_EQUATION(-1);
#endif

v[0]=0;
CYCLE_SAFE(cur, "EnergyFirm")
 {
  CYCLES(cur, cur1, "CapitalEF")
   {
   v[1]=VS(cur1,"ShareENPP");
   v[0]+=v[1];
   }
 }
RESULT(v[0] )




EQUATION("ControlENEF")
/*
*/
//v[10]=t;sprintf(msg, "\n ControlTotEN(%g)", v[10]); plog(msg);

#ifdef NOCHECKS
END_EQUATION(-1);
#endif

V("SumEnergyProductionPP");
V("SumEnergyProductionEF");
V("PriceEN");

v[0]=V("EnergyProductionEF");
v[1]=V("MaxEnergyCapacity");
v[3]=v[0]-v[1];

//if(v[0]>v[1])
//	INTERACT("ControlENEF",v[3]);

RESULT(v[3] )



EQUATION("ShareCapacityPP")
/*
*/
//v[10]=t;sprintf(msg, "\n aa(%g)", v[10]); plog(msg);
v[0]=V("SumMaxEnergyCapacity");
v[1]=V("KENCapacity");
v[10]=v[1]/v[0];
RESULT(v[10] )


EQUATION("ShareENPP")
/*
*/
//v[10]=t;sprintf(msg, "\n aa(%g)", v[10]); plog(msg);
V("PriceEN");
v[0]=V("SumEnergyProductionPP");
v[1]=V("EnergyProductionPP");
v[3] = V("FairEnergyPP");
v[10]=(v[1]+v[3])/v[0];
RESULT(v[10] )


EQUATION("ExcessCostPP")
/*
Excess cost due to overutilization
*/

V("PriceEN");

v[0]=V("ExcessENPP");
if(V("EFType")==0)
 v[1]=V("PriceENBrown");
else
 v[1]=V("PriceENGreen");

v[10]=v[0]*v[1];
RESULT(v[10] )


EQUATION("ExcessCostEF")
/*
Excess cost due to overutilization
*/
//v[0]=SUM("ExcessCostPP");
V("PriceEN");
v[0]=0;
CYCLE(cur, "CapitalEF")
 {
 	v[1]=VS(cur,"ExcessCostPP");
  v[0]+=v[1];
 }
RESULT(v[0] )


EQUATION("ExcessENPP")
/*
Excess production due to overutilization
*/
V("PriceEN");
v[0]=V("KENCapacity");
v[2] = V("FairEnergyPP");
v[1]=V("EnergyProductionPP");
v[10]=v[1]+v[2]-v[0];
if(v[10]<0)
	v[10]=0;
RESULT(v[10] )



EQUATION("TotExcessCost")
/*
Total excess cost to be distrubuted to classes
*/
v[0]=0;

CYCLE(cur, "EnergyFirm")
{
	v[0] += VS(cur, "ExcessCostEF");
}


RESULT(v[0] )


EQUATION("ExcessCostIncome")
/*
Excess energy cost due to overutilization distrubuted to classes
*/
v[0]=V("TotExcessCost");
v[1]=VL("ShareIncome",1);
v[10]=v[0]*v[1];
RESULT(v[10] )






EQUATION("KExpCostEN")
/*
*/
v[1]=V("PriceENMA");
v[2]=V("CurrentEfficiency");

v[0]=v[1]/v[2];

RESULT(v[0] )


EQUATION("KExpCostLabor")
/*
*/
v[1]=V("MinWageMA");
v[2]=V("CurrentProductivity");

v[0]=v[1]/v[2];

RESULT(v[0] )


EQUATION("CumBPI")
/*
Comment
*/
V("ControlDemand");
v[0] = VL("BankProfitIncome", 1);

RESULT(CURRENT+v[0] )


EQUATION("msEN")
/*
Market share of EN firms
*/
v[0] = V("TotENCapacity");
v[1] = V("MaxEnergyCapacity");
WRITE("msENCapacity", v[1]/v[0]);
v[2] = V("SumEnergyProductionEF");
v[3] = V("EnergyProductionEF");
RESULT(v[3]/v[2] )


EQUATION("ExitEF")
/*
Remove energy firms with too poor Health
*/

V("PriceEN");
//v[0]=t;
//sprintf(msg, "\n ExitEF(%g)", v[0]); plog(msg);
V("Exit"); // to be sure that NonPerformingLoansTotal is reset by Exit equation
if(V("NoExitEF")==1)
 END_EQUATION(0);

v[4]=v[70]=0;
v[18]=V("minAgeExitEF");
v[19]=V("minRRoKExitEF");
WRITE("CapitalDestroyedEF", 0); 
CYCLE_SAFE(cur, "EnergyFirm")
 {
  v[6]=VS(cur,"WaitingEF");
  v[16]=VS(cur,"AgeEF");
  v[17] = VS(cur, "pruned");
  v[36] = VS(cur, "MaxEnergyCapacity");
  v[37] = V("TotENCapacity");
  v[38] = V("minSize");
  if( v[6]!=-1 && v[16]>v[18] && v[17]==0) //don't kill new firms waiting for their first capital good to be delivered.
   {
    v[17]=VS(cur,"RRoKEF");
    v[7] = VS(cur, "CashEF");

    if( (v[17]<v[19] && v[7]<0) || (v[36]>0 && v[36]/v[37] < v[38]))
     {
     v[39]=v[36]/v[37];
//     if(v[39]>0.2)
  //    INTERACTS(cur, "Kill Big", v[39]);
       if(v[6]==1)
        {//firm is waiting an order, to be removed
         v[30] = VS(cur, "IdFirmEN");
         cur2 = SEARCH_CND("IdClient", -v[30]);
         cur2=cur->hook;
         if(cur2->hook!=cur)
          INTERACTS(cur, "Error exit EF", v[30]);
         if(cur2==NULL)
          INTERACTS(cur, "Err. exiting waiting firm", v[30]);
         else
          {
           if(INCRS(cur2->up, "NumOrders",-1)==0)
            {
             WRITES(cur2,"KAmount",0);
             WRITES(cur2,"KCompletion",0);
             WRITES(cur2,"TimeWaited",0);
             WRITES(cur2,"Kproductivity",0);
             WRITES(cur2,"KEfficiency",0);
             WRITES(cur2, "IdClient", -1);
             cur2->hook=NULL;
            }
           else
            {
             
             DELETE(cur2);
            }
          }
        }
       v[4]-=v[7];
      if(V("ExitFlagEF")==1 )
        INTERACTS(cur,"Dying EF", v[17]);
      v[21]=v[20]=0;
      CYCLES(cur, cur1, "CapitalEF")
      {
      	v[20]+=VS(cur1, "PrincipalEF");
      	v[21]+= VS(cur1, "KExpendituresEN");
      }
		 v[4]+=v[20];
     INCR("CapitalDestroyedEF", v[21]);
			CYCLES(cur, cur6, "LaborEF")
			{
				DELETE(cur6->hook);
			}

			v[31]=VS(cur,"IdFirmEN");
			
				if(VS(cur,"EFType")==0)
					{ // it's a brown firm
					CYCLES(cur, cur6, "CapitalEF")
						{
						if(cur6->hook!=NULL)
							{
							if(cur6->hook == NULL)
							INTERACTS(cur6, "NULL cur6->hook EXitEF", 1);

							v[55]=VS(cur6,"KExpendituresEN");
							v[56]=V("KOandMParaBrown");
							v[58]=VS(cur6,"MaxPPKAgeBrown");

							v[59] = VS(cur6, "IdCur");

							cur8 = SEARCH_CNDS(GRANDPARENT, "IdKFirm", v[59]);
							if(cur8 == NULL)
								INTERACTS(cur8, "NULL cur6 EXitEF", 1);
/**************************
							cur7 = SEARCHS(cur8, "BankK");
							INCRS(cur7,"KOandMRevenue",-(v[55]*v[56])/v[58]);
***************************/
							DELETE(cur6->hook); // remove the PP owned by the exited firm
							}
						else
							{
							INTERACTS(cur6, "Null PP", 1);
							}
						}
					}
				else
					{ // it's a green firm
					CYCLES(cur, cur6, "CapitalEF")
						{
						if(cur6->hook!=NULL)
							{
							v[55]=VS(cur6,"KExpendituresEN");
							v[56]=V("KOandMParaGreen");
							v[58]=VS(cur6,"MaxPPKAgeGreen");

							v[59] = VS(cur6, "IdCur");
							cur8 = SEARCH_CNDS(GRANDPARENT, "IdKFirm", v[59]);
/*******************************
							cur7 = SEARCHS(cur8, "BankK");
							INCRS(cur7,"KOandMRevenue",-(v[55]*v[56])/v[58]);
********************************/
							DELETE(cur6->hook); // remove the PP owned by the exited firm
							}
						else
							{
							INTERACTS(cur6, "Null PP", v[0]);
							}
						}
					}
			if(VS(cur, "pruned")==1)
			 INTERACTS(cur, "Killing pruned firm", v[0]);		
			DELETE(cur); // Delete the firm
     }
   }
 }
cur5 = SEARCHS(p->up,"Bank");
INCRS(cur5,"NonPerformingLoansTotal",v[4]);

v[66] = V("BankCapital");
if(v[66]>0)
 WRITE("RatioNPLEBC", v[4]/v[66]);
else
 WRITE("RatioNPLEBC", -1);

RESULT(v[4] )


EQUATION("AgeEF")
/*
*/
v[1]=VL("AgeEF",1);
RESULT(v[1]+1 )


EQUATION("KShareEngiProcess")
/*
*/
v[1]=V("KType");
v[2]=V("KShareEngiProcessEff");
if(v[1]==2)
 {
 	v[0]=0.1;
 }
else
 {
	v[0]=1-v[2];
 }
RESULT(v[0] )


EQUATION("ControlKShareEngi")
/*
*/
v[1]=V("KShareEngiProcess");
v[2]=V("KShareEngiEff");
v[3]=V("KShareEngiProd");
v[4]=V("KShareEngiProcessEff");

v[0]=1-v[1]-v[2]-v[3]-v[4];

RESULT(v[0] )



EQUATION("PriceENMAGr")
/*
Growth rate of PriceENMA
*/
v[1]=V("PriceENMA");
v[2]=VL("PriceENMA",1);

RESULT(v[1]/v[2]-1 )


EQUATION("MinWageMAGr")
/*
Growth rate of MinWageMA
*/
v[1]=V("MinWageMA");
v[2]=VL("MinWageMA",1);

RESULT(v[1]/v[2]-1 )



EQUATION("KShareEngiProd")
/*
Comment
*/
v[1]=V("PriceENMAGr");
v[2]=V("MinWageMAGr");

v[3]=V("KShareEngiProcess");
v[4]=V("KShareEngiProcessEff");

v[21]=VL("KExpCostEN",1);
v[22]=VL("KExpCostLabor",1);
v[23]=v[21]+v[22];

v[24]=v[21]/v[23]; // share of cost due to energy
v[25]=v[22]/v[23]; // share of cost due to labor

v[6]=VL("KShareEngiProd",1);

//v[7]=v[2]-v[1];
v[7]=v[2]*v[25]-v[1]*v[24];

v[10]=v[6]*(1+v[7]+ 0.01*(RND - 0.5) );

v[5]=1-v[3]-v[4]-v[10];
if(v[5]<0)
	END_EQUATION(v[6] ); //

RESULT(v[10] )



EQUATION("KShareEngiEff")
/*
Comment
*/
v[1]=V("KShareEngiProd");
v[2]=V("KShareEngiProcess");
v[3]=V("KShareEngiProcessEff");

v[10]=1-v[1]-v[2]-v[3]; // available engineers

RESULT(v[10] )



EQUATION("KNbrEngiProd")
/*
Defines the number of engineers working on vintage productivity
*/

v[0]=VL("KNbrEngineers",1);
v[1]=V("KShareEngiProd");
v[2]=v[0]*v[1];

RESULT(v[2] )

EQUATION("KNbrEngiEff")
/*
Defines the number of engineers working on vintage efficiency
*/

v[0]=VL("KNbrEngineers",1);
v[1]=V("KShareEngiEff");
v[2]=v[0]*v[1];

RESULT(v[2] )


EQUATION("KNbrEngiProcessEff")
/*
Defines the number of engineers working on process efficiency
*/

v[0]=VL("KNbrEngineers",1);
v[1]=V("KShareEngiProcessEff");
v[2]=v[0]*v[1];

RESULT(v[2] )



EQUATION("KNbrEngiProcess")
/*
Defines the number of engineers working on process
*/

v[0]=VL("KNbrEngineers",1);
v[1]=V("KShareEngiProcess");
v[2]=v[0]*v[1];

RESULT(v[2] )



EQUATION("CurrentProductivity")
/*
Changes in the productivity of kapital thanks to R&D
*/

v[0]=VL("CurrentProductivity",1);
v[1]=V("KNbrEngiProd");
v[2]=V("z");
v[3]=V("ProductivityShock");
v[4]=-(v[1]*v[2]);
v[5]=1-exp(v[4]);//Proba to innovate increases with the nbr of engineers
//first random draw to define the outcome of R&D i.e. success or failure
if (RND<v[5])
	{
	v[6]=norm(0,v[3]);//second stochastic variable, defines the level of productivity out of the successful R&D process
	}
else
	{
	v[6]=0;
	}
v[7]=max(v[6],0);
v[8]=v[0]*(1+v[7]);


v[12] = V("KType");
if(v[12]!=2)
 {
  v[9] = VL("EnergyFrontierProd",1);
  v[10] = V("speedSO");
  v[11]= v[8]*v[10]+(1-v[10])*v[9];
 }
else
 {
  v[11]=v[8];
 }
RESULT(v[11] )

EQUATION("EnergyFrontierProd")
/*
Energy productivity frontier
*/

v[0]=0;
CYCLE(cur, "KFirm")
{
	v[1] = VS(cur, "KType");
	if(v[1]!=2)
	 {
	  v[2] = VS(cur,"CurrentProductivity");
	  if(v[0]<v[2])
	   v[0]=v[2];
	 }
}

RESULT(v[0] )


EQUATION("CurrentEfficiency")
/*
Changes in the efficiency of kapital thanks to R&D
*/

v[0]=VL("CurrentEfficiency",1);
v[1]=V("KNbrEngiEff");
v[2]=V("z");
v[3]=V("EfficiencyShock");
v[4]=-(v[1]*v[2]);
v[5]=1-exp(v[4]);//Proba to innovate increases with the nbr of engineers
//first random draw to define the outcome of R&D i.e. success or failure
if (RND<v[5])
	{
	v[6]=norm(0,v[3]);//second stochastic variable, defines the level of efficiency out of the successful R&D process
	}
else
	{
	v[6]=0;
	}
v[7]=max(v[6],0);
v[8]=v[0] + v[7];

RESULT(v[8] )

EQUATION("KMaxEnergyEfficiency")
/*
R\&D to improve the energy efficiency of capital good production
*/

v[0]=VL("KMaxEnergyEfficiency",1);
v[1]=V("EnergyEfficiencyShock");
//v[2]=VL("KNbrEngiProcessEff",1);
v[2]=V("KNbrEngiProcessEff");
v[3]=V("z");
v[4]=-(v[2]*v[3]);
v[5]=1-exp(v[4]);//Proba to innovate increases with the nbr of engineers
//first random draw to define the outcome of R&D i.e. success or failure
if (RND<v[5])
	{
	v[8]=norm(0,v[1]);//second stochastic variable, defines the level of productivity out of the successful R&D process
	}
else
	{
	v[8]=0;
	}
v[10]=max(v[8],0);
v[11]=v[0] + v[10];
RESULT(v[11] )




EQUATION("KLaborProductivity")
/*
R\&D to improve labor productivity in K process
*/

v[0]=VL("KLaborProductivity",1);
v[1]=V("KProcessShock");
//v[2]=VL("KNbrEngiProcess",1);
v[2]=VL("KNbrEngiProcess",0);
v[3]=V("z");
v[4]=-(v[2]*v[3]);
v[5]=1-exp(v[4]);//Proba to innovate increases with the nbr of engineers
//first random draw to define the outcome of R&D i.e. success or failure
if (RND<v[5])
	{
	v[8]=norm(0,v[1]);//second stochastic variable, defines the level of productivity out of the successful R&D process
	}
else
	{
	v[8]=0;
	}
v[10]=max(v[8],0);
v[11]=v[0] + v[10];
RESULT(v[11] )


























EQUATION("alphaSize")
/*
Increasing return to scale
*/
v[1]=V("MaxEnergyCapacity");
v[2]=V("FactorSize");
v[0]=1+log(v[1]+1)/v[2];
RESULT(v[0] )


EQUATION("AvKENProductivity")
/*
Average productivity of K for EN firms
*/
v[0]=v[1]=0;
CYCLE(cur, "CapitalEF")
{
  v[2] = VS(cur, "KENProductivity");
  v[3] = VS(cur, "KENCapacity");
  v[0]+=v[2]*v[3];
  v[1]+=v[3];
}

if(v[1]!=0)
 v[2]=v[0]/v[1];
else
 v[2]=0;
RESULT(v[2] )

EQUATION("MarketEnergyCapacity")
/*
Total Energy Capacity
*/
v[0]=0;
CYCLE(cur, "EnergyFirm")
{
 v[0] += VS(cur, "MaxEnergyCapacity");
}

RESULT(v[0])

EQUATION("PercUnusedENCapacity")
/*
Percentage of unused EN capacity
*/

v[0] = V("TotENCapacity");
v[1] = V("SumEnergyProductionEF");
v[2]=(v[0]-v[1])/v[0];
RESULT( v[2] )

EQUATION("TotENCapacity")
/*
Function computing Green and Brown capacities
*/
v[0]=v[1]=v[2]=v[3]=v[4]=v[10]=v[11]=0;
V("PriceEN"); //ensure the computation of energy production and allocation of capital
CYCLE(cur, "EnergyFirm")
 {
	if(VS(cur, "EFType")==0 )
	 {
    v[3] += VS(cur, "EnergyProductionEF");
		v[0]+= VS(cur,"MaxEnergyCapacity");
		v[10]+=VS(cur, "AvKENProductivity");
	 }
	if(VS(cur, "EFType")==1 )
	 {
    v[4]+= VS(cur, "EnergyProductionEF");
		v[1]+= VS(cur,"MaxEnergyCapacity");
		v[11]+=VS(cur, "AvKENProductivity");
	 }
 }
WRITE("TotBrownCapacity",v[0]);
WRITE("TotGreenCapacity",v[1]);

WRITE("AvBrownProductivity",v[10]);
WRITE("AvGreenProductivity",v[11]);

v[5] = V("SumEnergyProductionPP");
WRITE("ShareBrownEnergy", v[3]/v[5]);
WRITE("ShareGreenEnergy", v[4]/v[5]);

RESULT(v[0]+v[1] )

EQUATION("CallalphaGreen")
/*
Decreasing return to scale
*/
V("TotENCapacity");
v[1]=V("TotGreenCapacity");
v[2]=V("FactorGreen");
v[0]=pow((1-v[2]),v[1]);
WRITE("alphaGreen", v[0]);
//INTERACT("CallalphaGreen",v[0]);
RESULT(1 )


EQUATION("CallalphaBrown")
/*
Decreasing return to scale
*/
V("TotENCapacity");
v[1]=V("HistoricalCapitalEN");
v[2]=V("FactorBrown");
v[0]=pow((1-v[2]),v[1]);
WRITE("alphaBrown", v[0]);
//INTERACT("CallalphaBrown",v[0]);
RESULT(1 )


EQUATION("ExpCostENK")
/*
Expected cost to produce energy without margin and management
*/
v[12]=V("MinWage");
v[4]=V("CurrentProductivity"); //current state of the K art
v[16]= v[12]/v[4]; // labor cost
v[6]=V("KPrice");
v[17]=v[6]; // Principal
v[18]=V("TimeRepaymentEF");
v[97]=v[17]/v[18];
v[11]=V("sInterestRate");
v[19]=v[11]*v[17]; // Interests

v[7]=(v[16]+v[97]+v[19]); // expected cost

RESULT(v[7] )





EQUATION("EntryEF")
/*
Entry of new energy firms
*/
// v[0]=t; //sprintf(msg, "\n EntryEF(%g)", v[0]); plog(msg);
//V("PriceEN");


cur1=ADDOBJ("EnergyFirm");
//INTERACT("New EnergyFirm added",v[1]);
cur1->hook=c;
WRITELS(cur1,"BalanceEF",0, t);
WRITELS(cur1,"CashEF",0, t);
WRITES(cur1,"DebtEF",0);
WRITES(cur1,"FossilFuel",0);
WRITES(cur1,"IdFirmEN",MAX("IdFirmEN")+1);
WRITES(cur1,"IdTechEN",1);
WRITES(cur1,"InvestmentSpendingEF",0);
WRITES(cur1,"KapitalNeedEF",0);
WRITES(cur1,"NumPP",0);
WRITELS(cur1,"ProfitEF",0, t);
WRITES(cur1,"WaitingEF",-1);
WRITES(cur1,"sBalanceEF",0);
WRITES(cur1,"sCashEF",0);
WRITES(cur1,"MaxKExpenditureEF",99999);
WRITES(cur1,"OutstandingDebtEF",0);

WRITES(cur1,"SafetyCoefEF",0.1);

v[0]=t;
v[2]=V("StepGreenPremium");
if(RND < 0.5 || v[0]<v[2]) // New EnergyFirm can be either brown (0) or green (1)
  v[1]=0;
else
  v[1]=1;

//if(t < V("StepGreenEntry"))
//	v[1]=0;
//else
//  v[1]=1;




WRITES(cur1,"EFType",v[1]);

cur2=SEARCHS(cur1,"CapitalEF");
WRITELS(cur2,"PPKAge",0, t);
WRITELS(cur2,"KENCapacity",0, t);
WRITES(cur2,"KENProductivity",1);
WRITES(cur2,"EnergyProductionPP",0);
WRITES(cur2, "FairEnergyPP", 0);
WRITES(cur2,"MAUtilizationPP",1);
WRITES(cur2,"KExpendituresEN",0);
WRITES(cur2,"CapitalEN",0);
WRITELS(cur2,"PrincipalEF",0, t);
WRITES(cur2,"ShareENPP",0);


//cur3=SEARCHS(cur1,"Engineers");
//cur4=SEARCH_CND("NumClass",0);
//cur3->hook=cur4;

cur3=SEARCH_CNDS(cur1,"IdLaborEF",1);
cur4=SEARCH_CND("NumClass",1);
cur5 = ADDOBJS(cur4, "LabClass");
cur5->hook=cur3;
cur3->hook=cur5;
WRITES(cur5, "LCType", 3);

cur2=SEARCHS(cur1,"LaborEF");
WRITELS(cur2,"NumWorkersEF",0,t);
WRITES(cur2,"PremiaEF",0);

cur=ADDOBJS(cur1,"LaborEF");
WRITES(cur,"IdLaborEF",2);
cur4=SEARCH_CND("NumClass",2);
cur5 = ADDOBJS(cur4, "LabClass");
cur5->hook=cur;
cur->hook=cur5;

WRITES(cur5, "LCType", 3);

cur5=SEARCH_CND("IdLaborEF",2);//
v[78]=VS(cur5,"wagecoeffEF");
WRITES(cur,"wagecoeffEF",v[78]);

v[4]=V("MinWage");
CYCLES(cur1, cur2, "LaborEF")
 {
  v[5]=VS(cur2,"wagecoeffEF");
  v[6]=v[5]*v[4];
  WRITELS(cur2,"wageEF",v[6],t);
  v[4]=v[6];
  WRITELS(cur2,"NumWorkersEF",0,t);
 }


//cur3=SEARCHS(cur1,"BankEF");
//cur4=SEARCHS(p->up,"Bank");
//if(cur4==NULL)
//	INTERACT("cur4 NULL", v[0]);
//if(cur3==NULL)
//	INTERACT("cur3 NULL", v[0]);

//cur3->hook=cur4;

// The new firm place an order to get capital

//VS(cur1,"InvestmentDecisionEF");


/*
WRITES(cur1, "KapitalNeedEF", 1000);

cur = SEARCHS(p->up->up, "Machinery");
WRITES(cur, "IsEnergyK", 1);
V("markupEFF");
WRITES(cur1,"WaitingEF",1);
*/


//INTERACT("EntryEF",v[0]);
RESULT( 1)





EQUATION("RemoveEFOrders")
/*
*/
CYCLE(cur, "KFirm")
	{
	CYCLE_SAFES(cur, cur1, "Order")
		{
		//cur2 = SEARCH_CNDS( cur1, "EnergyKOrder", 1 );
		v[1]=VS(cur1, "EnergyKOrder");
		if(v[1]==1 & COUNTS(cur,"Order")>1)
			{
			DELETE(cur1);
			//sprintf(msg, "\n Removed Order %lf", v[1] ); plog(msg);
			}
		}
	}

PARAMETER;
RESULT(1 )


EQUATION("SumWaitingEF")
/*
Number of firms waiting for an ordered K to be delivered
*/
v[0]=0;
CYCLE(cur, "EnergyFirm")
{
 v[1] = VS(cur,"WaitingEF");
 if(v[1]==1)
  v[0]++;
}

RESULT(v[0] )

EQUATION("SumRevenuesEF")
/*
*/
V("FinancialTrading");
v[0]=v[1]=0;
CYCLE(cur, "EnergyFirm")
	{
  v[0]=VS(cur,"RevenuesEF");
	v[1]+=v[0];
	}
RESULT(v[1] )



EQUATION("PriceENMA")
/*
Moving average of PriceEN
*/
v[1]=V("PriceEN");
v[2]=VL("PriceENMA",1);
v[3]=V("SpeedPriceENMA");

v[0]=v[3]*v[2]+(1-v[3])*v[1];
if(t==1)
 v[0]=v[1];
RESULT(v[0] )


EQUATION("ExpectedIncomePP")
/*
*/
v[2]=VL("PriceENMA",1);
v[3]=V("KapitalNeedEF");

v[99]=V("EFType");
if(v[99]==0)
 { // its a brown firm
	V("CallalphaBrown");
	v[41]=V("alphaBrown");
 }
else
 { // its a green firm
	V("CallalphaGreen");
	v[41]=V("alphaGreen");
 }

v[42]=V("alphaSize");

//v[0]=v[3]*v[2];
v[0]=v[3]*v[2]*v[41]*v[42];

RESULT(v[0] )


EQUATION("MinWageMA")
/*
Moving average of minwage
*/
v[1]=V("MinWage");
v[2]=VL("MinWageMA",1);
v[3]=V("SpeedMinWageMA");

v[0]=v[3]*v[2]+(1-v[3])*v[1];
RESULT(v[0] )








EQUATION("PlaceOrderEF")
/*
Place the order from the calling firm to a Kapital producer adopting the technology of the firm
*/
 //v[0]=t;
 //sprintf(msg, "\n PlaceOrderEF %lf", v[0] ); plog(msg);

//INTERACT("PlaceOrderEF",v[0]);

//v[44]=VLS(c,"NetWorth",1);

 v[0]=t;
 v[1] = V("stopPlaceOrderEF");
//sprintf(msg, "\n PlaceOrder %lf", v[0] ); plog(msg);
if(v[0]>v[1])
 END_EQUATION(0);

  /* v[72]=VS(c,"IdFirmEN");
   //sprintf(msg, "\n IdFirmEN %lf", v[72] ); plog(msg);
*/

v[82]=v[71]=V("IsEnergyK");

v[0]=VS(c,"IdTechEN"); //this is the technology of the energy firm
v[30]=VS(c,"betaPriceEN");
//sprintf(msg, "\n betaPriceEN %lf", v[30] ); plog(msg);

v[31]=VS(c,"betaProdEN");
v[32]=VS(c,"betaTimeEN");
//v[11]=VS(c,"betaEffEN"); //add betaEff as a parameter
v[11]=1;//assuming energy firms are not interested in the consumption of energy
v[3]=VS(c,"KapitalNeedEF");
//sprintf(msg, "\n KapitalNeedEF %lf", v[3] ); plog(msg);
//INTERACT("KapitalNeedEF",v[3]);

//v[33] = VS(c, "MaxEnergyCapacity");

v[74] = VS(c, "MaxKExpenditureEF");

v[99]=VS(c, "EFType");


//assuming there are many firms producing K with the same technologies, firm select the one they prefer in terms of price and productivity of the capital, and waiting time (insert also durability of the capital if we include depreciation as a function of production quantity and not time)
v[51]=v[53]=v[59]=v[60]=v[58]=v[70]=v[11]=v[12]=v[13]=v[14]=v[15]=v[16]=v[17]=v[18]=0;
//given the preference of the buyer firm wth respect to the features of the capital production, namely price and current productivity of the capital, and approximate time to wait for receiving the order & the energy efficiency


CYCLE(cur, "KFirm")
{

	v[98]=VS(cur,"KType");
  if(v[98]==v[99])
  {
  v[50]=VLS(cur,"KPrice",1);
  cur2=SEARCHS(cur,"KCapital");
  v[52]=VLS(cur2,"CurrentProductivity",1);
  //v[12]=VLS(cur2,"CurrentEfficiency",1); // Efficiency of the vintage

	v[51]+=v[50];
  v[53]+=v[52];
  //v[13]+=v[12];
  v[60]++;

  v[64]=VS(cur,"NumOrders");

  v[54]=VS(cur,"KQ");//number of productive workers
  // WRITES(cur,"WaitTime",ceil(v[3]/v[54]));
  WRITES(cur,"WaitTime",1);
  //if(v[64]>0)
   //{
     CYCLES(cur, cur1, "Order")
     {
       v[55]=VS(cur1,"KCompletion");
       v[56]=VS(cur1,"KAmount");
       v[57]=(v[56]-v[55])/v[54];
       INCRS(cur,"WaitTime",v[57]);
     }
   //}

  v[65]=VS(cur,"WaitTime");
  if(v[65]<0)
    INTERACTS(c, "Neg. waittime", v[65]);
  v[58]+=v[65];
  // an index that gives the amount of time needed or a firm to complete the production of the capital already ordered
	}
}
// write the average values for price  and producitivty of capital, and for the index of awaiting time. The averates are updated each time a firm place an order, as this would affectthe choice of a following firm in the same time period.
// NOTE THAT THIS PROVIDES A FIRST MOVER 'ADVANTAGE' TO THE FIRST FIRMS.
v[61]=v[51]/v[60];
WRITE("AvKPrice",v[61]);
v[62]=v[53]/v[60];
//WRITE("AvCurrProd",v[62]);
v[14]=v[13]/v[60];
WRITE("AvCurrEff",v[14]); // Average Current Efficiency
v[63]=v[58]/v[60];
WRITE("AvWaitTime",v[63]);


//v[17]=V("MinWage");
//v[18]=V("PriceEN");
v[81]=-99999;
CYCLE(cur, "KFirm")
{
  v[98]=VS(cur,"KType");
  if(v[98]==v[99])
  {
  v[1]=VS(cur,"IdKTech");
  if(v[0]!=v[1])
    WRITES(cur,"kapp",0);
  else
    WRITES(cur,"kapp",1);
  v[20]=VLS(cur,"KPrice",1);
  v[21]=VLS(cur,"CurrentProductivity",1);
  v[15]=VLS(cur,"CurrentEfficiency",1); // retrieve current efficiency
  v[26]=VS(cur,"WaitTime");
  // normalise relative values (shares) in order to compare firms on the different indicators using the same evaluation for different units of measurement
  v[27]=v[20]/v[61]+1; // Normalize Price
  v[28]=v[21]/v[62]+1; // Normalize Productivity
  v[16]=v[15]/v[14]+1; // Normalize Efficiency
  v[29]=v[26]/v[63]+1; // Normalize Wait Time
  if(v[27]*v[28]*v[29]*v[16]==0)
    v[33]=0;
  else// Do not include energy price and minimum wage here
    {
    v[33]=pow(v[28],v[31]) - pow(v[29],v[32]) - pow(v[27],v[30]); //sprintf(msg, "\n Grade %lf", v[33] ); plog(msg);
    //v[33]=RND;
		}
    //v[33]=pow(v[28],v[31])*pow(v[29],-v[32])*pow(v[27],-v[30])*pow(v[16],v[11]);
  //v[33]=pow(v[28]*v[17],v[31])*pow(v[29],-v[32])*pow(v[27],-v[30])*pow(v[16]*v[18],v[11]); // Includes PriceEN and MinWage
  WRITES(cur,"kselect",v[33]*VS(cur,"kapp"));
  v[70]+=v[33];
  if(v[33]>=v[81])
    {
    v[81]=v[33];
    cur1=cur;
    }
  }
}
      //Legend:
      //c: it is the final producer firm ordering the K
      //cur: is the K producer

v[6]=VLS(cur1,"KPrice",1);
if(VS(c, "WaitingEF")==-1)
  v[63]=v[3];//new entrants have no financial limits to invest
else
  v[63]=min(v[3],v[74]/v[6]); // v[3]=V("KapitalNeed")
 //sprintf(msg, "\n v63 %lf", v[63] ); plog(msg);

cur=cur1;

// Cost expectation

v[12]=V("MinWage");
v[4]=VLS(cur,"CurrentProductivity",1); //current state of the K art


v[16]= v[63]/v[4]*v[12]; // labor cost


v[17]=v[6]*v[63]; // Principal
v[18]=V("TimeRepaymentEF");
v[97]=v[17]/v[18];
v[11]=VL("sInterestRate",1);
v[19]=v[11]*v[17]; // Interests
v[2]=V("SafetyCoefEF");
v[7]= (1+v[2]) * (v[16]+v[97]+v[19]); // expected cost
v[8]=VS(c,"ExpectedIncomePP");

//sprintf(msg, "\n ExpectedIncomePP %lf", v[8] ); plog(msg);
//sprintf(msg, "\n Expected cost %lf", v[7] ); plog(msg);



if(v[8] > v[7] || v[8]>0)
	{
   if(VS(cur,"NumOrders")==0)
     cur1=SEARCHS(cur,"Order");
   else
     cur1=ADDOBJS(cur,"Order");

   WRITES(cur1,"KAmount",v[63]);

  //sprintf(msg, "\n E. Order Placed %lf", v[63] ); plog(msg);

   if(v[63]<=0)
     INTERACTS(c,"Neg.KAmountEF",v[63]);

   WRITES(cur1,"KCompletion",0);
   WRITES(cur1,"TimeWaited",1);
   WRITES(cur1, "EnergyKOrder", 1);
   cur1->hook=c; //useful to retrieve quickly the ordering firm
   cur2 = SEARCHS(c, "CapitalEF");
   INCRS(cur,"NumOrders",1);
   v[4]=VLS(cur,"CurrentProductivity",1); //current state of the K art
   WRITES(cur1,"Kproductivity",v[4]); //tech characteristics of the capital stock order
   WRITES(cur1,"KP",v[6]);// write the price of the capital in the period in which it is ordered, and use it to compute the actual expenditure using the `agreed' price.
	 WRITES(c,"WaitingEF",1);
   c->hook=cur1;

   v[72]=VS(c,"IdFirmEN");
   //sprintf(msg, "\n IdFirmEN %lf", v[72] ); plog(msg);
   WRITES(cur1,"IdClient",-v[72]);
	}
else
	{ //sprintf(msg, "\n neg. profitability %lf", v[63] ); plog(msg);
	END_EQUATION(1 );
	}

RESULT(1 )


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

EQUATION("UpdateExpenses")
/*
Variable ensuring that RdExpenditure and WageEngineers are updated, used in ControlDemand
*/

CYCLE(cur,"Firm")
	{
		VS(cur,"RdExpenditure");
		VS(cur,"WageEngineers");
	}

RESULT(1 )

EQUATION("ControlDemand")
/*
Control the vertical SFC of households
*/
/*****************
WRONG, firms are not located below Class
CYCLE(cur,"Firm")
	{
		VS(cur,"RdExpenditure");
		VS(cur,"WageEngineers");
	}
*****************/
V("UpdateExpenses");

V("Production");

v[0]=V("PremiaIncome");
v[1]=V("WageIncome");
v[2]=VL("InterestDepositsC",0);
v[3]=VL("BankProfitIncome",1); // redundant
v[4]=VL("OverdraftPaymentC",0);
v[5]=V("Expenditure");
v[6]=V("CashC");
v[7]=VL("CashC",1);
v[9] = V("CashCAdjustment");
v[8]=v[6]-v[7]+v[9];
v[18] = V("BalanceC");
if(abs(v[8]-v[18])/max(abs(v[8]),abs(v[18]))>0.0001)
  INTERACT("eRR BalanceC",v[18]);
v[20]=V("ExcessCostIncome");

v[23]=V("IncomeTax");
v[21] = V("Income");
if(abs((v[18]-(v[21]-v[5]))/abs(v[21]) )>0.5)
 INTERACT("Err Bal.2", v[18]);
v[22]=v[21]-v[0]-v[1]-v[2]-v[3]+v[4]-v[20]+v[23];
if(abs(v[22])/abs(v[21])>0.0001)
  INTERACT("eRR Income", v[22]);

//v[10]=v[0]+v[1]+v[2]+v[3]-v[4]-v[5]-(v[6]-v[7])+v[20];
//v[10]=v[0]+v[1]+v[2]+v[3]-v[4]-v[5]-v[8]+v[20]-v[23];
v[10]=v[0]+v[1]+v[2]+v[3]-v[4]-v[5]-v[8]+v[20]-v[23];

//v[20]=t; //sprintf(msg, "\n ControlDemand(%g)", v[20]); plog(msg);
RESULT(v[10])

EQUATION("CashCAdjustment")
/*
Variation of CashC due to Bank's actions
*/

V("BankCapital");
v[0] = V("CashC");
v[1] = V("OrganicCashC");
v[2]=v[1]-v[0];
RESULT(v[2] )



EQUATION("FirmControlCurrent")
/*
*/
V("Trade");
V("FinancialTrading");
V("InvestmentDecision");
V("Production");



//v[0]=V("Revenues");
v[0]=V("MonetarySales"); //flo

v[1]=V("WagePrem");
v[2]=V("LaborCost");
v[3]=V("RdExpenditure");
v[4]=V("ENCost");
v[6]=V("CapitalCostF");
v[7]=VL("OverdraftPaymentF",0);
v[8]=VL("InterestDepositsF",0);
v[12]=V("CashF");
v[13]=VL("CashF",1);
v[14] = V("CashFAdjustment");
v[15]=V("TaxF");

v[20]=v[0]-v[1]-v[2]-v[3]-v[4]-v[6]-v[7]+v[8]-(v[12]-v[13])-v[14]-v[15];
if((v[0]>0 && abs(v[20]/v[0])>0.000001 ) || (v[0]<=0 && abs(v[20]>1) ))
	INTERACT("FirmControlCurrent",v[20]);
RESULT(v[20] )



EQUATION("FirmControlCapital")
/*
*/
V("Trade");
V("FinancialTrading");
V("InvestmentDecision");
V("Production");

v[6]=V("CapitalPaymentF");
v[5]=V("InvestmentSpending");
v[10]=V("OutstandingDebtF");
v[11]=VL("OutstandingDebtF",1);

v[20]=(v[10]-v[11])-v[5]+v[6];
RESULT(v[20] )



EQUATION("KFirmControlCurrent")
/*
*/
V("Trade");
V("FinancialTrading");
V("SumKOandMRevenue");

v[0]=V("KRevenues");
v[1]=V("KWagePrem");
v[2]=V("KLaborCost");
v[4]=V("KENCost");
//v[7]=VL("OverdraftPaymentKF",1);
//v[8]=VL("InterestDepositsKF",1);
v[7]=V("OverdraftPaymentKF");
v[8]=V("InterestDepositsKF");
v[12]=V("CashKF");
v[13]=VL("CashKF",1);
v[14]=V("CashKFAdjustment");
v[15]=V("TaxK");
v[16]=V("CashInjectionKF");
v[21]=V("KOandMRevenue");

v[20]=v[0]-v[1]-v[2]-v[4]-v[7]+v[8]-(v[12]-v[13])-v[14]-v[15]+v[16]+v[21]; // added KOandMRevenue

//v[20]=v[0]-v[1]-v[2]-v[4]-v[7]+v[8]-(v[12]-v[13])-v[14]-v[15]+v[16];

if( (v[0]>0 && abs(v[20])/v[0]>0.0000001) || (v[0]<=0 && abs(v[20])>1) )
	INTERACT("KFirmControlCurrent",v[20]);
RESULT(v[20] )



EQUATION("EFirmControlCurrent")
/*
*/
V("Trade");
V("FinancialTrading");
V("InvestmentDecision");
V("Production");
v[30]=V("PriceEN");
V("BalanceEF");
//V("DemographyEN");

v[31]=0;
CYCLE(cur, "CapitalEF")
{
v[31]+=VS(cur, "EnergyProductionPP")+VS(cur,"FairEnergyPP");
}


v[0]=V("RevenuesEF");
v[32]=v[31]*v[30]-v[0];
v[1]=V("WagePremEF");
v[2]=V("LaborCostEF");
//v[6]=V("CapitalCostEF");
v[6]=V("CapitalPaymentEF");
v[17]=V("InterestPaymentEF");

v[7]=VL("OverdraftPaymentEF",0);
v[8]=VL("InterestDepositsEF",0);
v[12]=V("CashEF");
v[13]=VL("CashEF",1);
v[14]=V("ExcessCostEF");
v[15]=V("CashEFAdjustment");
v[16]=V("TaxEF");

v[21]=V("SumKOandMExpenditure");

v[18]=V("BalanceEF");

v[20]=v[0]-v[1]-v[2]-v[6]-v[17]-v[7]+v[8]-(v[12]-v[13])-v[14]-v[15]-v[16]-v[21]; //added SumKOandMExpenditure

//v[20]=v[0]-v[1]-v[2]-v[6]-v[17]-v[7]+v[8]-(v[12]-v[13])-v[14]-v[15]-v[16];

//v[20]=v[0]-v[1]-v[2]-v[6]-v[17]-v[7]+v[8]-v[14]-v[16]-v[18]; // test with BalanceEF

//if(abs(v[20])>1000 && V("ExitFlagEF")==1)
 //INTERACT("Spike EF",v[20]);

if( (v[0]>0 && abs(v[20])/v[0]>0.00000001) || (v[0]<=0 && abs(v[20])>1))
	INTERACT("EFirmControlCurrent",v[20]);
RESULT(v[20] )



EQUATION("EFirmControlCapital")
/*
*/
V("Trade");
V("FinancialTrading");
V("InvestmentDecisionEF");
V("Production");
V("PriceEN");
//V("DemographyEN");


v[6]=V("CapitalPaymentEF");
v[5]=V("InvestmentSpendingEF");
v[10]=V("OutstandingDebtEF");
v[11]=VL("OutstandingDebtEF",1);

v[12]=V("OutstandingDebtEFCh");


//v[20]=(v[10]-v[11])-v[5]+v[6];
v[20]=v[12]-v[5]+v[6];

//if(abs(v[20])>0.001 )
	//INTERACT("EFirmControlCapital",v[20]);
RESULT(v[20] )

EQUATION("ControlInterestDeposit")
/*
Comment
*/

v[0]=v[1]=v[2]=v[3]=v[4]=v[5]=v[6]=v[7]=v[8]=0;
CYCLE(cur, "Firm")
{
	v[9] = VLS(cur, "CashF", 1);
	if(v[9]>0)
	 v[11]+=v[9];
	v[1] += VS(cur, "InterestDepositsF");

}
v[21] = V("SumInterestDepositsF");
v[31] = VL("TotalDepositsF",1);


CYCLE(cur, "Class")
{
	v[9] = VLS(cur, "CashC", 1);
	if(v[9]>0)
	 v[12]+=v[9];
	v[2] += VS(cur, "InterestDepositsC");

}
v[22] = V("SumInterestDepositsC");
v[32] = VL("TotalDepositsC",1);

CYCLE(cur, "KFirm")
{
	v[9] = VLS(cur, "CashKF", 1);
	if(v[9]>0)
	 v[13]+=v[9];
	v[3] += VS(cur, "InterestDepositsKF");

}
v[23] = V("SumInterestDepositsKF");
v[33] = VL("TotalDepositsK",1);

CYCLE(cur, "EnergyFirm")
{
	v[9] = VLS(cur, "CashEF", 1);
	if(v[9]>0)
	 v[14]+=v[9];
	v[4] += VS(cur, "InterestDepositsEF");

}
v[24] = V("SumInterestDepositsEF");
v[34] = VL("TotalDepositsEF",1);


v[25] = VL("StatTotalDeposits",1);
v[0]=v[1]+v[2]+v[3]+v[4];
v[26]=v[21]+v[22]+v[23]+v[24];
v[27]=v[11]+v[12]+v[13]+v[14];
RESULT(1 )


EQUATION("BankControlCurrent")
/*
*/
V("Trade");
V("FinancialTrading");
V("InvestmentDecision");
V("Production");

v[1]=V("TotalInterestCollected");
v[2]=V("TotalInterestDeposits");
v[3]=V("RetainedProfitsBank");
v[4]=V("DistributedProfitsBank");

v[6] = V("SumInterestDepositsF");
v[7] = V("SumInterestDepositsC");
v[8] = V("SumInterestDepositsKF");
v[9] = V("SumInterestDepositsEF");

v[10]=v[6]+v[7]+v[8]+v[9];
//v[7] = VL("BankCapital", 1);
//v[8] = V("BankCapital");

v[0]=v[1]-v[2]-v[3]-v[4];
v[5] = V("CheckFinBalance");
//LOG("\n%lf",v[5]);
RESULT(v[0] )



EQUATION("BankControlCapital")
/*
*/
V("Trade");
V("FinancialTrading");
V("Production");
//V("DemographyEN");

v[1]=V("SumCashC");
v[2]=VL("SumCashC",1);
v[3]=V("SumCashF");
v[4]=VL("SumCashF",1);
v[5]=V("SumCashEF");
v[6]=VL("SumCashEF",1);
v[7]=V("SumCashKF");
v[8]=VL("SumCashKF",1);
v[9]=V("SumOutstandingDebtFCh");
v[10]=V("SumOutstandingDebtEFCh");

v[0]=(v[1]-v[2])+(v[3]-v[4])+(v[5]-v[6])+(v[7]-v[8])-v[9]-v[10];
RESULT(v[0] )



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////






EQUATION("SumControlFirms")
/*
*/
RESULT(SUM("ControlFirms"); )



EQUATION("ControlRdExpenditure")
/*
*/

// v[10]=t;sprintf(msg, "\n ControlRd(%g)", v[10]); plog(msg);

CYCLE(cur,"Class")
 {
V("ControlDemand");
 }
V("KWageEngineers");

CYCLE(cur,"Firm")
	{
		VS(cur,"WageEngineers"); v[10]=t; //sprintf(msg, "\n ControlRd(%g)", v[10]); plog(msg);
	}
//V("WageEngineers"); // Does not work. ControlRd is executed before WageEngineers

v[0]=V("SumKRdExpenditure");
v[1]=V("SumRdExpenditure");

cur=SEARCH_CND("NumClass",0);
v[2]=VS(cur,"WageIncome");

v[3]=v[2]-v[0]-v[1];
//if(abs(v[3])>0.001)
 //INTERACT("RdExpenditure",v[3]);
//v[10]=t; //sprintf(msg, "\n ControlRd(%g)", v[10]); plog(msg);
RESULT(v[3] )



EQUATION("KENCost")
/*
*/
v[3]=V("KEN");
v[4]=V("PriceEN");
v[6]=v[3]*v[4]; //UNRECOGNISE GRAMMAR ERROR
RESULT(v[6] )



EQUATION("ENCost")
/*
*/
v[0]=V("EN");
v[1]=V("PriceEN");

RESULT(v[0]*v[1] )



EQUATION("ControlTotEN")
/*
*/
//v[10]=t;sprintf(msg, "\n ControlTotEN(%g)", v[10]); plog(msg);

V("SumEnergyProductionPP");
V("SumEnergyProductionEF");
V("PriceEN");

v[0]=V("TotEnergyConsumption");
v[1]=V("SumEnergyProductionEF");
//v[2]=V("ExcessEnergy");
v[3]=v[0]-v[1];//-v[2];
//if(v[3]>0.001 | v[3]<-0.001) // avoid approximation issue
	//INTERACT("ControlTotEN",v[3]);
RESULT(v[3] )



EQUATION("ControlInvestment") //ok
/*
*/

V("SumKRevenues"); // Necessary to be sure the ControlInvestment is executed after
V("SumInvestment"); // Necessary to be sure the ControlInvestment is executed after
V("SumInvestmentEF"); // Necessary to be sure the ControlInvestment is executed after

 v[0]=t;
//sprintf(msg, "\n ControlInvestment(%g)", v[0]); plog(msg);

v[0]=V("SumKRevenues"); //sprintf(msg, " SumKRevenues(%g)\n", v[0]); plog(msg);
v[1]=V("SumInvestment"); //sprintf(msg, " SumInvestment(%g)\n", v[1]); plog(msg);
v[2]=V("SumInvestmentEF"); //sprintf(msg, " SumInvestmentEF(%g)\n", v[2]); plog(msg);
v[3]=v[0]-v[1]-v[2]; //sprintf(msg, " v3(%g)\n", v[3]); plog(msg);
//if(v[3]>0.01 | v[3]<-0.01) // avoid approximation issue
	//INTERACT("ControlInvestment",v[3]);
RESULT(v[3] )



EQUATION("ControlLaborCost") //ok
/*
Ensure labor total costs equal total income
 */
V("Trade");

v[30]=V("ControlPremiaIncome");

v[0]=V("TotWage");
v[1]=V("SumLaborCost");
v[2]=V("SumKLaborCost");
v[3]=V("SumLaborCostEF");
v[4]=V("SumRdExpenditure");


v[6] = V("SumWagePrem");
v[7] = V("SumWagePremEF");
v[8] = V("SumKWagePrem");
v[9]=V("TotPremia");

v[5]=v[0]+v[9]-v[1]-v[2]-v[3]-v[4]-v[6]-v[7]-v[8];

v[20]=v[0]-v[1]-v[2]-v[3]-v[4];
v[21]=v[9]-v[6]-v[7]-v[8];

v[11]=v[9]+v[0];
v[10]=abs(v[5]/v[11]);
if(v[10]>0.000001) // avoid approximation issue
	INTERACT("ControlLaborCost",v[5]);

RESULT(v[5] )



EQUATION("ControlPremiaIncome") //ok
/*
 */
v[0]=V("SumPremiaIncome");
v[1]=V("SumWagePrem");
v[2]=V("SumKWagePrem");
v[3]=V("SumWagePremEF");
v[4]=v[0]-v[1]-v[2]-v[3];
//if(v[4]>0.001 | v[4]<-0.001) // avoid approximation issue
	//INTERACT("ControlPremiaIncome",v[4]);
RESULT(v[4] )



EQUATION("ControlPremia") // does not work because of add-on and removal of tiers
/*
*/

V("NumWorkers");
V("KNbrWorkers");

v[0]=v[1]=0;
CYCLE(cur, "Labor")
{
  v[0]=VS(cur,"Premia");
  v[1]+=v[0];
}
v[2]=V("WagePrem");
v[3]=v[1]-v[2];
if(v[3]>0.001 || v[3]<-0.001) // avoid approximation issue
	INTERACT("ControlPremia",v[3]);
RESULT(v[3] )



EQUATION("ControlKPremia") //ok
/*
*/
V("NumWorkers");
V("KNbrWorkers");

v[0]=v[1]=0;
CYCLE(cur, "KLabor")
{
  v[0]=VS(cur,"KPremia");
  v[1]+=v[0];
}
v[2]=V("KWagePrem");
v[3]=v[1]-v[2];
//if(v[3]>0.001 | v[3]<-0.001) // avoid approximation issue
	//INTERACT("ControlKPremia",v[3]);
RESULT(v[3] )



EQUATION("ControlPremiaEF") //ok
/*
*/
v[0]=v[1]=0;
CYCLE(cur, "LaborEF")
{
  v[0]=VS(cur,"PremiaEF");
  v[1]+=v[0];
}
v[2]=V("WagePremEF");
v[3]=v[1]-v[2];
//if(v[3]>0.001 | v[3]<-0.001) // avoid approximation issue
	//INTERACT("ControlPremiaEF",v[3]);
RESULT(v[3] )



EQUATION("ControlRevenues")//
/*
 */
V("Trade");
V("backlog");
V("BacklogValue");
V("Stocks");
V("UnitDemand");

v[10]=t;
//sprintf(msg, "\n ControlRevenues(%g)", v[10]); plog(msg);
v[0]=V("Revenues");
v[1]=V("MonetarySales");
v[10]=V("UnitSales");
v[11]=VL("price",1)+V("ExtraPrice");
v[12]=v[10]*v[11];
v[2]=V("BacklogValueCh");
//v[3]=v[0]-v[1]-v[2];
//v[3]=v[0]-v[12];//-v[2];
v[3]=v[0]-v[1];

//if(v[3]>0.01 | v[3]<-0.01) // avoid approximation issue
	//INTERACT("ControlRevenues",v[3]);
RESULT(v[3] )



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////




EQUATION("SumOutstandingDebtEFCh")
/*
*/
V("FinancialTrading");
v[0]=v[1]=0;
CYCLE(cur, "EnergyFirm")
	{
  v[0]=VS(cur,"OutstandingDebtEFCh");
	v[1]+=v[0];
	}
RESULT(v[1] )



EQUATION("OutstandingDebtEFCh")
/*
*/
V("Trade");
V("FinancialTrading");
V("InvestmentDecision");
V("Production");

v[10]=V("OutstandingDebtEF");
v[11]=VL("OutstandingDebtEF",1);

RESULT(v[10]-v[11] )



EQUATION("OutstandingDebtFCh")
/*

*/
V("Trade");
V("FinancialTrading");
V("InvestmentDecision");
V("Production");

v[10]=V("OutstandingDebtF");
v[11]=VL("OutstandingDebtF",1);
v[12]=v[10]-v[11];
RESULT(v[12] )



EQUATION("SumOutstandingDebtFCh")
/*
*/
V("FinancialTrading");
v[0]=v[1]=0;
CYCLE(cur, "Firm")
	{
  v[0]=VS(cur,"OutstandingDebtFCh");
	v[1]+=v[0];
	}
RESULT(v[1] )





EQUATION("ControlBankBS")
/*
Check on Bank balance sheet
*/

v[0]=V("TotalAssets");
v[1]=V("StatTotalDeposits");
v[2]=V("BankCapital");
v[4] = VL("DistributedProfitsBank", 1);

v[3]=v[0]-v[1]-v[2]-v[4];
//LOG("\n%lf\n%lf\n%lf \t %lf\n\n",v[0],v[1],v[2], v[3]);
v[40]=v[41]=v[42]=v[43]=v[44]=v[45]=v[46]=v[47]=0;
CYCLES(THIS->up, cur, "Firm")
{
 v[16]=VS(cur,"CashF");
  if(v[16]>0)
   v[40]+=v[16];
  else
   v[41]+=-v[16];
  v[41] += VS(cur, "OutstandingDebtF");
}

CYCLES(THIS->up, cur, "Class")
{
 v[16]=VS(cur,"CashC");
  if(v[16]>0)
   v[42]+=v[16];
  else
   v[43]+=-v[16];
}

CYCLES(THIS->up, cur, "KFirm")
{
 v[16]=VS(cur,"CashKF");
  if(v[16]>0)
   v[44]+=v[16];
  else
   v[45]+=-v[16];
}

CYCLES(THIS->up, cur, "EnergyFirm")
{
 v[16]=VS(cur,"CashEF");
  if(v[16]>0)
   v[46]+=v[16];
  else
   v[47]+=-v[16];
 v[47] += VS(cur, "OutstandingDebtEF");
}

v[48]=v[40]+v[42]+v[44]+v[46];
v[49]=v[41]+v[43]+v[45]+v[47];

if(abs(v[1]-v[48])>10)
 INTERACT("WRONG DEPOSITS", v[41]-v[48]);

v[20] = V("GovernmentFund");
//v[21] = V("NonPerformingLoansTotal");
v[21]=0;
if(abs(v[0]+v[20]-v[49]-v[21])>10)
 INTERACT("WRONG assets", v[0]+v[20]-v[49]-v[21]);

RESULT(v[3] )




EQUATION("BankProfitIncome")
/*
 */
V("FinancialTrading");
v[1]=VL("DistributedProfitsBank",1);
//v[2]=VL("CashC",1);
//v[3]=VL("SumCashC",1);
v[4]=VL("ShareIncome",1);

//v[0]=v[1]*(v[2]/v[3])
v[0]=v[1]*v[4];//xxx
RESULT(v[0] )



EQUATION("SumCashC")
/*
*/
V("FinancialTrading");
v[0]=v[1]=0;
CYCLE(cur, "Class")
	{
  v[0]=VS(cur,"CashC");
	v[1]+=v[0];
	}
RESULT(v[1] )



EQUATION("SumCashF")
/*
*/
V("FinancialTrading");
v[0]=v[1]=0;
CYCLE(cur, "Firm")
	{
  v[0]=VS(cur,"CashF");
	v[1]+=v[0];
	}
RESULT(v[1] )



EQUATION("SumCashEF")
/*
*/
V("FinancialTrading");
v[0]=v[1]=0;
CYCLE(cur, "EnergyFirm")
	{
  v[0]=VS(cur,"CashEF");
	v[1]+=v[0];
	}
RESULT(v[1] )



EQUATION("SumCashKF")
/*
*/
V("FinancialTrading");
v[0]=v[1]=0;
CYCLE(cur, "KFirm")
	{
  v[0]=VS(cur,"CashKF");
	v[1]+=v[0];
	}
RESULT(v[1] )



EQUATION("TotalOutstandingDebtV")
/*
variable of TotalOutstandingDebt (for calculating lag)
 */
 V("FinancialTrading");
v[0]=V("TotalOutstandingDebt");
RESULT(v[0] )



EQUATION("TotalProfitsBank")
/*
*/
V("FinancialTrading");

v[1] = V("TotalInterestCollected");
v[2] = V("TotalInterestDeposits");

v[0]= v[1]-v[2];
RESULT(v[0] )



EQUATION("TotalInterestDeposits")
/*
Interest paid on deposits
*/
V("FinancialTrading");
v[0] = V("SumInterestDepositsF");
v[1] = V("SumInterestDepositsC");
v[2] = V("SumInterestDepositsKF");
v[3] = V("SumInterestDepositsEF");

v[4] = v[0]+v[1]+v[2]+v[3];
RESULT(v[4] )



EQUATION("RetainedProfitsBank")
/*
*/
V("FinancialTrading");
v[1] = V("TotalProfitsBank");
v[2] = V("DistributedProfitsBank");

v[0] = v[1] - v[2];
RESULT(v[0] )



EQUATION("DistributedProfitsBank")
/*
Profits distributed to households.
Computed as the current profits minus a fixed return on bank's capital
*/
V("FinancialTrading");
v[3] = V("TotalProfitsBank"); // parameter
v[4] = V("FixedReturnOnCapitalBank");
v[2] = V("BankCapital");

v[1]=v[2]*v[4];
v[0]=max(0, min(v[3],v[1])); 

RESULT(v[0] )



EQUATION("OwnFundsTargetLRBank")
/*
*/
V("FinancialTrading");
v[1] = V("TotalAssets");
v[2] = V("CapitalAdequacyRatioTargetBank"); //parameter set in line with Basel III regulations

v[0] = v[1] * v[2];
RESULT(v[0] )



EQUATION("OwnFundsTargetSRBank")
/*
*/
V("FinancialTrading");
v[1] = VL("BankCapital",1);
v[2] = V("OwnFundsParameterBank"); //parameter that determines the proportion of the difference between the target and actual levels made up in each period
v[3] = V("OwnFundsTargetLRBank");

v[0] = v[1] + (v[2]* (v[3] - v[1] ));
RESULT(v[0] )



EQUATION("RetainedProfitsTargetBank")
/*
*/
V("FinancialTrading");
v[1] = V("OwnFundsTargetSRBank");
v[2] = VL("BankCapital",1);
v[3] = V("ExpectedLoanDefaultsPC");
v[4] = VL("TotalOutstandingDebtV",1);

v[0] = (v[1] - v[2]) + (v[3] * v[4]);
RESULT(v[0] )



EQUATION("TotalProfitsTargetBank")
/*
*/
V("FinancialTrading");
v[1] = V("RetainedProfitsTargetBank");
v[2] = V("DistributedProfitsBank");

v[0] = v[1] + v[2];
RESULT(v[0] )



EQUATION("InterestRateSpread")
/*
*/
V("FinancialTrading");
v[0] = VL("InterestRateSpread", 1);
v[1] = V("InterestRateDeposits")/10;//step to change of interest rate

v[2] = V("BankCapital");
v[3] = V("TotalAssets");

v[5]=v[2]/v[3];
v[4] = V("CapitalAdequacyRatioTargetBank");

v[8]=round(100*(v[4]-v[5]) )/100;

v[7]=v[0];
if(v[8]>0)
 v[7]+=v[1];//increase spread if capital is low
if(v[8]<0)
 v[7]-=v[1];//decrease spread if capital is high

v[6]=V("MinInterestRateSpread");
if(v[7]<v[6])
	v[7]=v[6];

	v[9]=V("MaxInterestRateSpread");
if(v[7]>v[9])
	v[7]=v[9];


RESULT(v[7] )



EQUATION("InterestRate")
/*
*/

v[1] = V("InterestRateDeposits");
v[2] = V("InterestRateSpread");

v[0] = v[1] + v[2];//xxx
RESULT(v[0] )



EQUATION("CapitalAdequacyRatio")
/*
*/
V("FinancialTrading");
v[1] = V("BankCapital");
v[2] = V("TotalAssets");

v[0] = v[1] / v[2];
RESULT(v[0] )


EQUATION("ExpectedLoanDefaultsPC")
/*
*/
V("FinancialTrading");


v[1] = VL("NonPerformingLoansTotal",1);
v[2] = VL("TotalAssets",1);
v[3] = VL("ExpectedLoanDefaultsPC",1);


v[0] = (0.9*v[3]) + (0.1*(v[1]/v[2]));
RESULT(v[0] )



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////






EQUATION("TotalAssets")
/*
Total values of assets
*/
V("FinancialTrading");
v[1] = V("TotalOverdraft");
v[2] = V("TotalOutstandingDebt");

v[3]=v[1]+v[2];
RESULT(v[3] )



EQUATION("CashEFCh")
/*
*/
//V("DemographyEN");
v[10]=V("CashEF");
v[11]=VL("CashEF",1);

v[20]=v[10]-v[11]; //UNRECOGNISE GRAMMAR ERROR
RESULT(v[20] )



EQUATION("CashFCh")
/*
*/
v[10]=V("CashF");
v[11]=VL("CashF",1);
v[12]=v[10]-v[11];
RESULT(v[12] )








//////////////////////////////////////////////////////////////////////////////////////////////////////////////////



EQUATION("KRevenues2")
/*
wrong, because KPrice is not the order's price
*/
v[0]=V("KPrice");
v[1]=V("KDelivered");
v[10]=v[0]*v[1];
RESULT(v[10] )



EQUATION("KENCost")
/*
*/
v[3]=V("KEN");
v[4]=V("PriceEN");
v[6]=v[3]*v[4]; //UNRECOGNISE GRAMMAR ERROR
RESULT(v[6] )




EQUATION("DebtFCh") //xxx
/*
Change of DebtF
*/
v[0]=V("DebtF");
v[1]=V("DebtFL");
RESULT(v[0]-v[1] )



EQUATION("DebtKCh") //xxx
/*
Change of DebtF
*/
v[0]=V("DebtK");
v[1]=V("DebtKL");
RESULT(v[0]-v[1] )

EQUATION("ENCost")
/*
*/
v[0]=V("EN");
v[1]=V("PriceEN");

RESULT(v[0]*v[1] )



EQUATION("KRdExpenditure")
/*
*/

V("KWageEngineers");

v[0]=V("MinWage");
v[1]=V("KEWagecoeff");
v[3]=V("KNbrEngineers");

RESULT((v[1]*v[0]*v[3]) )



EQUATION("SumKRdExpenditure")
/*
*/
V("KRdExpenditure");
RESULT(SUM("KRdExpenditure") )


EQUATION("SumRdExpenditure")
/*
*/
V("RdExpenditure");
RESULT(SUM("RdExpenditure") )




EQUATION("WageEngineers")
/*
Wage of engineers
*/

//v[10]=t;sprintf(msg, "\n WageEngineers(%g)", v[10]); plog(msg);


v[0]=V("MinWage");
v[1]=V("EWagecoeff");

v[3]=V("RdExpenditure");
//v[20]=t; //sprintf(msg, "\n WageEngineers(%g)", v[20]); plog(msg);
RESULT((v[0]*v[1]) )




EQUATION("NbrEngineers")
/*
Number of engineers
*/
v[0]=V("RdExpenditure");
v[1]=V("WageEngineers");
//INCRS(p->hook,"Individuals",v[0]/v[1]);
RESULT((v[0]/v[1]) )









EQUATION("MaxNumClass")
/*
*/
RESULT(MAX("NumClass") )



EQUATION("SumBalanceC")
/*
*/
v[0]=v[1]=0;
CYCLE(cur, "Class")
	{
  v[0]=VS(cur,"BalanceC");
	v[1]+=v[0];
	}
RESULT(v[1] )


EQUATION("SumBacklog")
/*
*/
v[0]=v[1]=0;
CYCLE(cur, "Firm")
	{
  v[0]=VS(cur,"backlog");
	v[1]+=v[0];
	}
RESULT(v[1] )


EQUATION("SumBacklogValueCh")
/*
*/
v[0]=v[1]=0;
CYCLE(cur, "Firm")
	{
  v[0]=VS(cur,"backlogValueCh");
	v[1]+=v[0];
	}
RESULT(v[1] )


EQUATION("SumBacklogCh")
/*
*/
v[0]=v[1]=0;
CYCLE(cur, "Firm")
	{
  v[0]=VS(cur,"backlogCh");
	v[1]+=v[0];
	}
RESULT(v[1] )





EQUATION("backlogCh")
/*
*/
v[0]=V("backlog");
v[1]=VL("backlog",1);
RESULT(v[0]-v[1] )



EQUATION("SumExpenditures")
/*
*/
v[0]=v[1]=0;
CYCLE(cur, "Class")
	{
  v[0]=VS(cur,"Expenditure");
	v[1]+=v[0];
	}
RESULT(v[1] )



EQUATION("SumBacklogSales")
/*
*/
v[0]=v[1]=0;
CYCLE(cur, "Firm")
	{
  v[0]=VS(cur,"backlogSales");
	v[1]+=v[0];
	}
RESULT(v[1] )




EQUATION("SumRevenues")
/*
*/
RESULT(SUM("Revenues") )



EQUATION("SumEnergyProductionEF")
/*
*/
V("PriceEN");
v[0]=0;
CYCLE(cur,"EnergyFirm")
	{
	v[1]=VS(cur,"EnergyProductionEF");
	v[0]+=v[1];
	}
RESULT(v[0] )

EQUATION("SumEnergyProductionPP")
/*
*/

//v[10]=t;sprintf(msg, "\n SumEnergyProductionPP(%g)", v[10]); plog(msg);

V("PriceEN");
v[0]=0;
CYCLE(cur,"EnergyFirm")
	{
	CYCLES(cur,cur1,"CapitalEF")
		{
		v[1]=VS(cur1,"EnergyProductionPP");
		v[2] = VS(cur1,"FairEnergyPP");
		v[0]+=v[1]+v[2];
		}
	}
RESULT(v[0] )


EQUATION("SumKRevenues")
/*
Sum of revenues of KFirms (Investment of Firms and EnergyFirms)
 */
 v[0]=t;
//sprintf(msg, "\n SumKRevenues(%g)", v[0]); plog(msg);
v[0]=v[1]=0;
CYCLE(cur, "KFirm")
{ VS(cur, "KProductionFlow");
  v[0]=VS(cur,"KRevenues");
  v[1]+=v[0];
}
RESULT(v[1] )



EQUATION("SumInvestment")
/*
Sum of investment spending of Firms
 */
 v[0]=t;
//sprintf(msg, "\n SumInvestment(%g)", v[0]); plog(msg);
v[0]=v[1]=0;
CYCLE(cur, "Firm")
{
  v[0]=VS(cur,"InvestmentSpending");
  v[1]+=v[0];
}
RESULT(v[1] )



EQUATION("SumInvestmentEF")
/*
Sum of investment spending of EnergyFirms
*/
 v[0]=t;
//sprintf(msg, "\n SumInvestmentEF(%g)", v[0]); plog(msg);
v[0]=v[1]=0;
CYCLE(cur, "EnergyFirm")
{
  v[0]=VS(cur,"InvestmentSpendingEF"); //sprintf(msg, " InvestmentSpendingEF(%g)\n", v[0]); plog(msg);
  v[1]+=v[0]; //sprintf(msg, " total(%g)\n", v[1]); plog(msg);
}
RESULT(v[1] )



EQUATION("SumLaborCost")
/*
Sum of wages distributed by Firms
 */
v[0]=v[1]=0;
CYCLE(cur, "Firm")
{
  v[0]=VS(cur,"LaborCost");
  v[1]+=v[0];
}
RESULT(v[1] )



EQUATION("SumKLaborCost")
/*
Sum of wages distributed by KFirms
 */
v[0]=v[1]=0;
CYCLE(cur, "KFirm")
{
  v[0]=VS(cur,"KLaborCost");
  v[1]+=v[0];
}
RESULT(v[1] )



EQUATION("SumLaborCostEF")
/*
Sum of wages distributed by Energy Firms
 */
v[0]=v[1]=0;
CYCLE(cur, "EnergyFirm")
{
  v[0]=VS(cur,"LaborCostEF");
  v[1]+=v[0];
}
RESULT(v[1] )



EQUATION("SumWagePrem")
/*
Sum of Premia distributed by Firms
 */
v[0]=v[1]=0;
CYCLE(cur, "Firm")
{
  v[0]=VS(cur,"WagePrem");
  v[1]+=v[0];
}
RESULT(v[1] )



EQUATION("SumKWagePrem")
/*
Sum of Premia distributed by KFirms
 */
v[0]=v[1]=0;
CYCLE(cur, "KFirm")
{
  v[0]=VS(cur,"KWagePrem");
  v[1]+=v[0];
}
RESULT(v[1] )



EQUATION("SumWagePremEF")
/*
Sum of Premia distributed by EnergyFirms
 */
v[0]=v[1]=0;
CYCLE(cur, "EnergyFirm")
{
  v[0]=VS(cur,"WagePremEF");
  v[1]+=v[0];
}
RESULT(v[1] )



EQUATION("SumPremiaIncome")
/*
Sum of Premia distributed to classes
 */
v[0]=v[1]=0;
CYCLE(cur, "Class")
{
  v[0]=VS(cur,"PremiaIncome");
  v[1]+=v[0];
}
RESULT(v[1] )



EQUATION("DebtFL") //xxx
/*
Lagged value of DebtF
 */
//v[10]=t; //sprintf(msg, "\n DebtFL(%g)", v[10]); plog(msg);

v[0]=V("DebtF");
RESULT(v[0] )


EQUATION("DebtKL")//xxx
/*
Lagged value of DebtK
 */
//v[10]=t; //sprintf(msg, "\n DebtFKL(%g)", v[10]); plog(msg);

v[0]=V("DebtK");
RESULT(v[0] )




EQUATION("OutstandingDebtEFCh")//xxx
/*
Change in DebtEF
 */
v[0]=V("OutstandingDebtEF");
v[1]=VL("OutstandingDebtEF",1);
RESULT(v[0]-v[1] )







EQUATION("AvMaxEfficiency")
/*
*/
v[1]=AVE("MaxEfficiency");

RESULT(v[1] )


EQUATION("ENGDP")
/*
*/
v[1]=V("TotEnergyConsumption");
v[2]=V("GDP");
if(v[2]==0)
	END_EQUATION(0 ); // Avoid 'ENGDP' produces the invalid value 'inf'

RESULT(v[1]/v[2] )



EQUATION("MinWageGr")
/*
*/
v[4]=VL("MinWage",1);
v[5]=V("MinWage");
v[7]=v[5]/v[4]-1; //compute the growth rate of MinWage
RESULT(v[7] )



EQUATION("GreenEnergyPriceProd")  // Called by KCapital
/*
Compute the investment cost to produce one unit of green energy during the whole life of the vintage.
*/

cur1=SEARCHS(p->up,"KFirm");

v[0]=VS(cur1,"KPrice");

// INTERACT("KPrice",v[0]);

v[1]=V("GreenProductivity");

// INTERACT("MaxENProduction",v[1]);

v[2]=v[0]/v[1];

RESULT(v[2] )



EQUATION("VacanciesRatio")
/*
*/
v[0]=V("Vacancies");
v[1]=V("NumWorkers");

RESULT(v[0]/v[1] )



EQUATION("LaborForceRatio")
/*
*/
v[0]=V("LaborForce");
v[1]=V("TotLaborForce");

RESULT(v[0]/v[1] )



EQUATION("aNW")
/*
*/
//v[0]=VL("MaxLaborProductivity",1);
//v[1]=VL("Q",1);
//v[2]=v[1]-v[0];

//if(v[2]<0)
//	v[2]=0;

v[3]=V("aaNW");

v[4]=V("Vacancies");



if(v[4]<1)
	v[4]=1;

v[5]=v[3]*log(v[4]);
if(v[5]>1)
	v[5]=1;

if(v[5]<V("aNWmin") )
 v[5]=V("aNWmin");
if(v[5]>V("aNWmax"))
 v[5]=V("aNWmax");

v[6]=VL("aNW",1);
//INTERACT("aNW 1", v[6]);
v[7]=V("aNWsmooth")*v[5]+(1-V("aNWsmooth"))*v[6];

//RESULT(v[3]*log(v[2]+1) )
RESULT(v[7] )







EQUATION("TotQ")
/*
*/
v[0]=SUM("Q");

RESULT(v[0] )

EQUATION("TotUnitDemand")
/*
*/
v[0]=SUM("UnitDemand");

RESULT(v[0] )

EQUATION("AvMarkup")
/*
*/
v[0]=AVE("markup");

RESULT(v[0] )

EQUATION("SumKNbrWorkers")
/*
*/
v[0]=SUM("KNbrWorkers");

RESULT(v[0] )

EQUATION("TotKLaborForce")
/*
*/
v[0]=SUM("KLaborForce");

RESULT(v[0] )

EQUATION("TotLaborForce")
/*
*/
v[0]=SUM("LaborForce");

RESULT(v[0] )

EQUATION("AvMaxLaborProductivity")
/*
*/
v[0]=AVE("MaxLaborProductivity");

RESULT(v[0] )

EQUATION("TotBacklog")
/*
*/
v[0]=SUM("backlog");
RESULT(v[0] )

EQUATION("TotStocks")
/*
*/
v[0]=SUM("Stocks");
RESULT(v[0] )

EQUATION("AvDiffCapacity")
/*
*/
v[0]=AVE("DiffCapacity");

RESULT(v[0] )

EQUATION("DiffCapacity")
/*
*/
v[0]=V("CapitalCapacity");
v[1]=V("LaborCapacity");

RESULT(v[0]-v[1] )


EQUATION("NbrFirms")
/*
Count the number of firm waiting for their new capital
*/
v[0]=COUNT("Firm");

RESULT(v[0] )

EQUATION("NbrNotWaiting")
/*
Count the number of firm waiting for their new capital
*/
v[0]=SUM("Waiting");
v[1]=COUNT("Firm");
v[2]=v[1]-v[0];

RESULT(v[2] )


EQUATION("NbrWaiting")
/*
Count the number of firm waiting for their new capital
*/
v[0]=SUM("Waiting");

RESULT(v[0] )



EQUATION("TotInvestment")
/*
*/

v[0]=SUM("KProductionFlow");

RESULT(v[0] )



EQUATION("AvLocalPriceProd")
/*
*/
v[0]=V("AvKPrice");
v[1]=V("AvCurrProd");

RESULT(v[0]/v[1] )



EQUATION("GreenENShare")
/*
*/
v[0]=V("GreenEN");
v[1]=V("TotEnergyConsumption");

RESULT(v[0]/v[1] )

EQUATION("DirtyENShare")
/*
*/
v[0]=V("DirtyEN");
v[1]=V("TotEnergyConsumption");

RESULT(v[0]/v[1] )




EQUATION("maxCostUnitEnergy")
/*
Comment
*/
v[0]=-1;
CYCLE(cur, "EnergyFirm")
{
	v[1] = VS(cur, "CostUnitEnergy");
	if(v[0]< v[1])
	 v[0]=v[1];
}

RESULT(v[0] )


EQUATION("InitPP")
/*
Initalize the objects required to compute the price of energy. A hook links every PP object to a CapitalEF object in one of the energy producers
*/
v[0]=0;

CYCLE(cur, "EnergyFirm")
{
	CYCLES(cur, cur1, "CapitalEF")
		v[0]++;
}

v[1]=0;
CYCLE(cur, "PP")
{v[1]++;
}
v[2]=v[0]-v[1];
if(v[2]>0)
 ADDNOBJ("PP", v[2]);
if(v[2]<0)
 INTERACT("Error in # of PP",v[2]);
CYCLE(cur, "PP")
	 WRITELS(cur, "PricePP", -2, 0);

cur2 = SEARCH("PP");
CYCLE(cur, "EnergyFirm")
{
	CYCLES(cur, cur1, "CapitalEF")
		{
		 cur2->hook=cur1;
		 cur1->hook=cur2;
		 cur2=cur2->next;
		 v[20]=VS(cur1,"IdFirmEN");
		 WRITES(cur1->hook, "IdEF", v[20]);
		}
}
//CYCLE(cur, "PP")
// {
//	v[20]=VS(cur2->hook,"IdFirmEN");
//	WRITES(cur, "IdEF", 0);
// }

PARAMETER
RESULT(1 )


EQUATION("FairEnergy")
/*
Share of energy demand distributed in proportion to capacity and not on competitive bidding process
*/
v[0] = V("shareFairEnergy");
v[1] = V("TotEnergyConsumption");
v[3]=v[0]*v[1];
v[2] = V("SumMaxEnergyCapacity");
if(v[2]<v[3])
 v[3]=0.95*v[2];
RESULT(v[3] )

EQUATION("FairEnergyPP")
/*
FairEnergy for capital unit, a PP
*/
v[0] = V("FairEnergy");
v[1] = V("KENCapacity");
v[2] = V("MarketEnergyCapacity");
RESULT(v[0]*v[1]/v[2] )


EQUATION("PriceEN")
/*
Uniform price of energy computed as the cost of the marginal firm ranked according to increasing unit costs of production
*/

v[30]=V("ExcessEnergy");
WRITE("ExcessEnergyL", v[30]); // save lagged value of ExcessEnergy
WRITE("ExcessEnergy", 0); // reset ExcessEnergy
v[40]=V("SumMaxEnergyCapacity");
v[5]=v[0]=V("TotEnergyConsumption") - V("FairEnergy");
WRITE("CopyTotalEnergy", v[0]);
//CYCLE(cur, "PP")
	//VS(cur, "PricePP");

//sprintf(msg, "\n TotEnergyConsumption(%g)", v[5]); plog(msg);


//SORT("PP", "PricePP", "UP");
SORT("PP", "PricePPDoped", "UP");

CYCLE(cur, "PP")
{
	WRITES(cur->hook, "EnergyProductionPP", 0); // reset EnergyProductionPP
	WRITES(cur->hook, "ShareENPP", 0);

}


v[4]=v[7]=v[30]=v[31]=v[32]=v[41]=v[42]=v[43]=v[44]=0;
v[3]=V("PricePP"); //
CYCLE(cur, "PP")
{
	v[1]=VS(cur->hook,"KENCapacity") - VS(cur->hook, "FairEnergyPP");
	if(v[1]<0)
	 INTERACTS(cur, "Neg. production: excessive high demand", v[1]);
	v[2]=max(min(v[0],v[1]) , 0);
	WRITES(cur->hook, "EnergyProductionPP", v[2]);
	v[0]-=v[2];
	if(v[2]>0)
	{
	if(VS(cur->hook, "EFType")==0)
	 {
	  v[41]+=v[2]; //brown
	  v[43] = VS(cur, "PricePP");//highest brown price because the PP are ranked for increasing PricePP levels
	 }
	else
	 {
 	  v[42]+=v[2]; //green energy
	  v[44] = VS(cur, "PricePP");//highest green price because the PP are ranked for increasing PricePP levels
	 }
	}
	WRITES(cur->hook, "ShareENPP", v[2]);//numerator of the share of en for the pp

	v[30] = VS(cur, "PricePP"); // record the last producer

	if(v[7]==0 ) // energy demand is satisfied
	  {
	  v[3] = VS(cur, "PricePP"); // record the price for non-null producers
   	v[4]+=v[2];
   	if(v[0]==0)
   	 v[7]=1;
   	}
}

v[43]=v[44]=v[3];/************** REMOVE!!!!!!!!*********************/
WRITE("PriceENBrown", v[43]);
WRITE("PriceENGreen", v[44]);

v[3]=(v[43]*v[41]+v[44]*v[42])/(v[41]+v[42]);
//sprintf(msg, "\n ExcessEnergy(%g)", v[0]); plog(msg);
WRITE("ExcessEnergy", v[0]);
//v[10]=t;sprintf(msg, "\n ExcessEnergy(%g)", v[10]); plog(msg);

v[90]=0;
if(v[0]>0)
 {
	CYCLE(cur, "PP")
	 {
		v[20] = MULTS(cur->hook, "ShareENPP", 1/(v[41]+v[42]));//share of competitively distributed energy
		v[21]=v[20]*v[0]; // Distribute excess demand to PP proportionaly to their capacity
		INCRS(cur->hook, "EnergyProductionPP", v[21]);
		v[90]+=v[21];
	 }

 }


RESULT(v[3])


EQUATION("EnergyProductionEF")
/*
Total energy produced by the firm
*/
V("PriceEN");
v[0]=0;
CYCLE(cur, "CapitalEF")
{
	v[0]+=VS(cur, "EnergyProductionPP")+VS(cur,"FairEnergyPP");
}
RESULT(v[0] )


EQUATION("RevenuesEF")
/*
Revenues of Energy Firms
*/

v[0]=V("PriceEN");
if(V("EFType")==0)
 v[0] = V("PriceENBrown");
else
 v[0] = V("PriceENGreen");

v[1]=V("EnergyProductionEF");


RESULT(v[0]*v[1] )






EQUATION("MAUtilizationPP")
/*
Moving average of PP utilization
*/
V("PriceEN");//to ensure that the current production is set
v[0] = VL("MAUtilizationPP", 1);
v[1] = V("EnergyProductionPP");
v[5] = V("FairEnergyPP");
v[3] = V("SpeedMA");
v[4] = V("KENCapacity");


if(v[4]==0) // this avoid to scrap pp of new entrants
	v[2]=1;
else
  v[2]=v[3]*v[0]+(1-v[3])*(v[1]+v[5])/v[4];
//	v[2]=v[3]*v[0]+(1-v[3])*(v[1])/(v[4]-v[5]);
	//v[2]=v[3]*v[0]+(1-v[3])*(v[1]+v[5])/v[4];
RESULT(v[2] )


EQUATION("GreenPremium")
/*
Energy rice reduction due to the use of green capital
*/
v[0] = VL("GreenPremium", 1);
v[1] = V("GPstep");


if(t < V("StepGreenPremium"))
	v[10]=v[0];
else
  v[10]=v[0]+v[1];

v[2] = V("minGPremium");
if(v[10]<v[2])
 v[10]=v[2];

RESULT(v[10] )

EQUATION("PricePPDoped")
/*
Competitive price of energy producers.
It is based on the standard price reduced in case it uses green capital
*/

v[0] = V("PricePP");
v[1] = VS(p->hook, "EFType");
if(v[1]==0)
 v[3]=v[0]; // it's a brown firm
else
 { // it's a green firm
  v[2] = VS(p->up, "GreenPremium");
  v[3]=v[0]*v[2];
 }
RESULT(v[3])

EQUATION("PriceEK")
/*
Price of the power plant
*/
V("PriceEN");
if(p->hook!=NULL)
 v[0] = VS(p->hook, "PricePP");
else
 v[0]=0;
RESULT(v[0] )


EQUATION("shareLaborKEN")
/*
Share of workers used in the capital
*/
v[0] = V("InitKENCapacity");
v[1] = V("KENProductivity");
v[2] = V("RequiredWorkersEF");
if(v[2]>0)
 v[3]=(v[0]/v[1])/v[2];
else
 v[3]=0;
RESULT(v[3] )

EQUATION("UnitCostKEN")
/*
Cost of units of energy computing labor and capital
*/


v[0] = VS(p, "shareLaborKEN");
v[1]=VS(p,"LaborCostEF");
v[2] = VS(p,"markupEF");
v[3] = VS(p, "InitKENCapacity");

if(v[3]>0)
 v[4]=v[0]*v[1]/v[3];
else
 v[4]=0; 
v[5]=v[4]*v[2];
END_EQUATION(v[5])

v[0] = V("LaborCostEF");//total labor cost
v[1] = V("shareLaborKEN");
v[2] = V("InstallmentEF");
v[3] = V("InterestEF");

v[8] = V("OverdraftPaymentEF");
v[9] = V("MaxEnergyCapacity");

v[5] = V("KENCapacity");

v[4]=v[8]*v[5]/v[9]+v[3]+v[2]+v[0]*v[1]; // total cost

v[11] = V("InitKENCapacity");

if(v[5]>0)
 v[6]=v[4]/v[5];
else
 v[6]=0;
RESULT(v[6] )

EQUATION("PricePP")
/*
Price computed as the markup on the cost of unit of energy
for this capital unit
*/

v[0] = VS(p->hook, "shareLaborKEN");
v[1]=VS(p->hook,"LaborCostEF");
v[2] = VS(p->hook,"markupEF");
v[3] = VS(p->hook, "InitKENCapacity");

v[4]=v[0]*v[1]/v[3];
v[5]=v[4]*v[2]+uniform(-0.0001, 0.0001);
END_EQUATION(v[5])

v[11]=VS(p->hook,"SumKOandMExpenditure");

v[12] = V("MaxEnergyCapacity");

v[11]=VL("PricePP",1);
v[12] = V("smoothPPP");
//v[13]=v[11]*v[12]+(1-v[12])*v[4];
v[13]=v[11]*v[12]+(1-v[12])*v[21];
if(t==1)
 v[13]=v[21];
RESULT(v[13] )

EQUATION("markupEF")
/*
Markup for energy firms. Decreases in case of null sales and increases otherwise
*/
v[0] = VL("ProfitEF",1);
v[6] = V("MaxMUEF");
v[7] = V("MinMUEF");
v[1] = VL("markupEF", 1);
v[2] = V("alphaMUEF");
if(v[0]>0)
 v[3]=v[2]*v[1]+(1-v[2])*v[6]; //maximum markup=2
else
 v[3]=v[2]*v[1]+(1-v[2])*v[7]; //minimum markup=1

if(V("AgeEF")<=1)
 v[3]=v[6];
RESULT(v[3] )

EQUATION("MaxEnergyCapacity")
/*
Maximum capacity
*/


v[0]=0;
CYCLE(cur, "CapitalEF")
{
	v[1] = VS(cur, "KENCapacity");
	v[0]+=v[1];
}

RESULT(v[0] )


EQUATION("LaborCostEF")
/*
Labor cost of Energy Firms
*/
v[0]=0;

CYCLE(cur, "LaborEF")
 {
  v[1]=VS(cur,"NumWorkersEF");
  v[2]=VS(cur,"wageEF");
  v[0]+=v[1]*v[2];
 }

RESULT(v[0] )

EQUATION("NumWorkersEF")
/*
Number of workers, working for EnergyFirms
*/

v[15]=0;
CYCLES(p->up, cur, "LaborEF")
 { //check how many tiers already exist
  v[15]++;
 }
v[14]=V("IdLaborEF");
if(v[14]==1)
 { // compute the first tier workers given their productivity and production needs
  v[6]=V("RequiredWorkersEF");
 }
else
 {// when above the first tier workers...
  v[18]=V("IdLaborEF");
  cur=SEARCH_CNDS(p->up,"IdLaborEF",v[18]-1);
  v[21]=VS(cur,"nuEF"); //given the worker ratio between tiers (defined by the tier below)
  v[19]=VS(cur,"NumWorkersEF"); //and the number of workers in the previous tier
  v[6]=v[19]/v[21]; // compute the required executives for the current tier
  v[17]=V("nuEF");
  if(v[6]>=v[17] && v[18]==v[15])
   { // if they are above the workers ratio of this tier and this is the last tier, create a new working class
    cur1=ADDOBJ_EXS(p->up,"LaborEF",p);
    WRITES(cur1,"IdLaborEF",v[18]+1);
    v[20]=v[6]/v[17];
    WRITELS(cur1,"NumWorkersEF",v[20], t);
    WRITELLS(cur1,"NumWorkersEF",0, t,1); // write also that the number of workers in the previous period is equal to 0, as it is used in the inequality statistics
    cur3=SEARCH_CND("NumClass",v[18]+1);
    if(cur3==NULL)
    	{
//			INTERACT("NULL cur3 missing class",v[1]);
			v[22]=0;
    CYCLES(p->up->up->up,cur2, "Class")
      v[22]++;
		if(v[22]-1<v[18]+1 )
    { //starting from the second class (the first are engineers), if it does not exist a class that represnt the new layer of executives, create it
      cur2=SEARCH_CNDS(p->up->up->up,"NumClass",v[18]);
      cur3=ADDOBJ_EXS(cur2->up,"Class", cur2);
      v[31]=0;
      CYCLE_SAFES(cur3, cur4, "LabClass")
      {
       if(v[31]==0)
        {
         v[31]=1;
         WRITES(cur4, "LCType", 0);
        }
       else
        DELETE(cur4); 
      }

      //cur3=ADDOBJS(cur2->up,"Class");
      INTERACTS(cur3,"Labor EF New class",v[22]);
      cur8=SEARCHS(cur3,"BankC");
      WRITELS(cur8,"BalanceC",0, t-1);
      WRITELS(cur8,"OverdraftPaymentC",0, t-1);

      cur8->hook=cur7;
      
      cur5 = ADDOBJS(cur3, "LabClass");
      cur5->hook=cur1;
      cur1->hook=cur5;
      WRITES(cur5, "LCType", 3);

      
      v[44]=VS(cur2,"SavingRate");
      v[45]=VS(cur2->up,"SRMultiplier");
      v[47] = VS(cur2->up, "TopSavingRate");
      v[46]=v[44]*v[45]+(1-v[45])*v[47];
      WRITES(cur3,"SavingRate",v[46]);
      //      INTERACTS(cur3,"Inspect SavingRate", v[46]);

      WRITES(cur3,"NumClass",v[18]+1);
      WRITELLS(cur3,"Expenditure",0, t-1,1);
      WRITELS(cur3,"Expenditure",0, t);
      WRITELS(cur3,"Income",0, t-1);
      WRITELS(cur3,"WageIncome",0, t-1);
      WRITELS(cur3,"PremiaIncome",0, t-1);
      WRITELS(cur3,"Consumption",0, t-1);
      WRITELS(cur3,"GovDemHH",0, t-1);
      WRITELS(cur3,"ShareWageIncome",0, t-1);
      WRITELS(cur3,"SharePremiaIncome",0, t-1);
      WRITELS(cur3,"BankProfitIncome",0, t-1);
      WRITELS(cur3, "CashC", 0, t-1);
      WRITELS(cur3,"ShareIncome",0, t-1); // reset the share income to be recomputed
      WRITELS(cur3,"Individuals",0, t-1); // set the number of individuals to nu;ber of workers of the new class
      v[35]=VS(cur2,"LorenzInd");
      WRITES(cur3,"LorenzInd",v[35]+v[20]); // set total number of workers as previous total plus new workers
      //WRITELS(cur3,"NoConsumption",0, t-1); // set the savongs due to unavailability of the good to 0
      //INTERACTS(cur3,"Created class",v[44]);
      CYCLES(cur3, cur, "Need")
      { // enter in neds and characterisitcs to change the tau parameter (the minimum is set every period)
        v[27]=VS(cur,"IdNeed");
        CYCLES(cur, cur1, "DCh")
        {
          v[26]=VS(cur1,"IdDCh");
          CYCLES(cur2, cur4, "Need")
          { // cycle among the needs of the preceding class
            v[28]=VS(cur4,"IdNeed");
            if(v[28]==v[27])
            { // when in the same need as the one the new class is cycling cycle through the char of the preceding class
              CYCLES(cur4, cur5, "DCh")
              {
                v[29]=VS(cur5,"IdDCh");
                if(v[29]==v[26])// when in the same characteristic the new class is cycling
                  v[30]=VS(cur5,"tau"); // read the value of the tau parameter
              }

            }
          }
          v[31]=VS(cur1,"tauMultiplier"); // the speed of adjustment of the tolerance level from one class to the following one
          v[34]=VS(cur1,"NegativeQuality");
          if(v[34]>0)
            v[33]=VS(p->up->up->up,"tauMax"); // the asympthotic level of the tolerance level (1 for qualities and 0 for price)
          if(v[34]<0)
            v[33]=VS(p->up->up->up,"tauMin"); // the asympthotic level of the tolerance level (1 for qualities and 0 for price)
          v[32]=v[30]*(1-v[31])+v[31]*v[33]; // adjustment in the treshold level of tolerance
          WRITES(cur1,"tau",v[32]); // finally write the tau for the new consumer class in each of its characteristic for each need
        }

      }
      v[35]=VS(cur3,"ComputeShare"); // set the distribution of expenditure shares across needs for the new class
      //INTERACTS(cur3,"NewClass END", v[20]);
    }
    else
    {
      
      cur5 = ADDOBJS(cur3, "LabClass");
      cur5->hook=cur1;
      cur1->hook=cur5;
      WRITES(cur5, "LCType", 3);
    }
    	}
    else
    	{
      
      cur5 = ADDOBJS(cur3, "LabClass");
      cur5->hook=cur1;
      cur1->hook=cur5;
      WRITES(cur5, "LCType", 3);
      }
   }
  if(v[18]>2 && v[19]<v[21])
   v[6]=0;
 }


RESULT(v[6] )


EQUATION("wageEF")
/*
Wage of the workers in this layer of the firm
*/
v[5]=V("NumWorkersEF");
v[2]=V("IdLaborEF");
if(v[2]==1)
 { // first tier workers
  v[0]=V("MinWage");
  v[1]=V("wagecoeffEF"); // wage coefficient as the minimum wage multiplier
 }
else
 { // executives
  cur=SEARCH_CNDS(p->up,"IdLaborEF",v[2]-1);
  v[0]=VS(cur,"wageEF");
  v[1]=V("wagecoeffEF"); // wage coefficient as the wage tier multiplier
 }


RESULT((v[0]*v[1]) )


EQUATION("RequiredWorkersEF")
/*
Compute the number of workers for the energy firms
*/

v[0]=0;
CYCLE(cur, "CapitalEF")
{
 v[1] = VS(cur, "KENProductivity");
 v[2] = VS(cur, "InitKENCapacity");
 v[0] +=v[2]/v[1];
}

RESULT(v[0] )


EQUATION("EN")
/*
Compute the energy used in production
*/

v[10]=V("Q");
v[0]=0;
v[1]=0;

CYCLE(cur, "Capital")
 {
  v[5]=VS(cur,"IncEfficiency");
  v[8] = VS(cur, "IncProductivity");
  v[7]=VS(cur,"FDKCapacity");//computes the actual stock of this capital vintage that can be used
  v[9]=v[7]*v[8]; //max production with this capital vintage
  v[11]=min(v[10], v[9]);//actual production with this vintage
  v[10]-=v[11]; //remaining production
  if(v[5]>0)
   v[0]+=v[11]/v[5];//energy consumed by this vintage
}

RESULT(v[0] )


EQUATION("UnitEnergyCost")
/*
Cost of energy for unit of output
*/

v[3]=VL("EN",1);
//v[4]=VL("PriceENMA",1);
v[4]=VL("PriceEN",1);
v[5]=VL("Q",1);

if(v[5]>0.01)
 {
	v[6]=(v[3]*v[4])/v[5]; // compute the energy cost to produce one final good using lagged values
 }
else
	{//you need an estimate of EN cost even when producing nihil
	 
	 v[20] = VL("UnitEnergyCost", 1);
	 END_EQUATION(v[20]);
	 v[0]=0;
   v[1]=0;
   
   CYCLE(cur, "Capital")
    {
     v[15]=VS(cur,"IncEfficiency");
     v[8] = VS(cur, "IncProductivity");
     v[7]=VS(cur,"FDKCapacity");//computes the actual stock of this capital vintage that can be used
     v[9]=v[7]*v[8]; //max production with this capital vintage
     v[0]+=v[9]/v[15];//energy consumed by this vintage
     v[1]+=v[9]; 
    }
   v[6]=v[0]*v[4]/v[9]; 
  // INTERACT("BOH", v[6]);
	}

RESULT(v[6] )

EQUATION("ClearingPrice")
/*
Comment
*/

v[0] = VL("price", 1);
v[1] = V("Q");
v[2] = V("Stocks");
v[3] = V("UnitDemand");
v[4] = V("UnitSales");
v[6] = V("aClearingPrice");
if(v[3]+v[4]==0)
 v[5]=-1;
else
 v[5]=v[0]*(1+v[6]*(v[3]-v[4])*2/(v[3]+v[4]));
RESULT(v[5] )

EQUATION("price")
/*
Markup on the unit production cost
*/
v[10]=V("markup");
v[22]=V("UnitLaborCost"); // labour in the first tier (the ones which define the production capacity)

v[6]=V("UnitEnergyCost");

v[14]=v[10]*(v[22]+v[6]); // add the energy cost to the pricing equation

v[30] = VL("price", 1);
v[31] = v[30]+VL("ExtraPrice",1);

v[33] = V("shiftExPrice");
v[32]=v[33]*v[31]+(1-v[33])*v[14];

RESULT(v[32] )

EQUATION("KEN")
/*
Compute the energy used by final good firms
*/
v[0]=V("KQ");
v[1]=V("KMaxEnergyEfficiency");
RESULT(v[0]/v[1] )

EQUATION("KUnitEnergyCost")
/*
comment
*/

v[3]=V("KEN");
v[4]=V("PriceEN");
v[5]=V("KQ");

if(v[5]>0.01)
 {
	v[6]=(v[3]*v[4])/v[5]; // compute the energy cost to produce one final good
 }
else
	v[6]=0;

RESULT(v[6] )





EQUATION("KPrice")
/*
Comment
*/
v[0]=V("Kmarkup");
v[8]=V("KQ"); // productive capacity of the firm
v[4]=V("KLaborCost");
v[1]=V("KUnitEnergyCost");
//v[7]=(v[0])*(v[4]/v[8]+v[1]);
v[7]=(v[0])*(v[4]/v[8]);

/**********************
v[9] = V("KRatioVacancies");
v[10] = V("KPriceOverCharge");
v[19]=max(v[9],10);
v[8]=v[19]*v[10];
v[17]=v[7]*(1+v[8]);
***********************/

if(v[7]<=0)
	INTERACT("Neg.KPrice",v[7]);

RESULT(v[7] )


EQUATION("TotEN")
/*
Total Energy Consumption of FIRMS
*/
RESULT(SUM("EN") )

EQUATION("TotEN2")
/*
Total Energy Consumption of FIRMS
*/
v[1]=v[2]=0;
CYCLE(cur,"Firm")
	{
	v[1]=VS(cur,"EN");
	v[2]+=v[1];
	}

RESULT(v[2] )

EQUATION("TotKEN")
/*
Total Energy Consumption of KFIRMS
*/
RESULT(SUM("KEN") )


EQUATION("TotKEN2")
/*
Total Energy Consumption of KFIRMS
*/
v[1]=v[2]=0;
CYCLE(cur,"KFirm")
	{
	v[1]=VS(cur,"KEN");
	v[2]+=v[1];
	}

RESULT(v[2] )


EQUATION("TotEnergyConsumption")
/*
Total Energy Consumption (Firms+Kfirms)
*/
//v[10]=t;sprintf(msg, "\n TotEnergyConsumption(%g)", v[10]); plog(msg);
v[0]=V("TotEN");
v[1]=V("TotKEN");

RESULT(v[0]+v[1] )


EQUATION("SumScrappedCapitalF")
/*
Total Capital Scrapped
*/
v[0]=0;
CYCLE(cur, "Firm")
{
	VS(cur, "CapitalStock");
	v[0] += VS(cur, "ScrappedCapital");
}

RESULT(v[0] )

EQUATION("CapitalCapacity")
// Comment
v[0] = V("CapitalStock");
v[2]=V("CapitalIntens");
v[1]=v[0]/v[2];
RESULT(v[1] )

EQUATION("CapitalStock")
/*
Compute the depreciation of capital
*/
v[0]=VL("CapitalStock",1);

v[20] = V("flagNewEntrant");
if(v[20]==1)
 END_EQUATION(v[0]);

WRITE("ScrappedCapital", 0);
v[2]=V("CapitalIntens");
v[10]=0;
//if(V("IdFirm")==895)
 //INTERACT("Merde 3", v[0]);

v[40] = round(V("CapitalLife")*(1+RND*0.4) );
v[41] = V("CapitalProdEndLife");
v[42]=(1-v[41])/v[40];

v[20] = V("TimeRepayment");
v[50]=0;
CYCLE(cur, "Capital")
	v[50]++;
WRITE("NumK", v[50]);
CYCLE_SAFE(cur, "Capital")
 {
  v[4]=min(VS(cur,"KAge"),v[40]);
  v[7] = VS(cur, "PrincipalF");
  v[8] = VS(cur, "InstallmentF");
  if( v[4]<v[40] || v[50]==1 )// Scrap if end of life
     {
      v[10]+=VS(cur, "FDKCapacity");
     }
    else
     {
      v[28]=VS(cur, "KExpenditures");
      INCR("ScrappedCapital", v[28]);
      if(v[7]>0 || v[8]>0)
       INTERACTS(cur, "Scraping unpaid capital",v[0]);
      DELETE(cur);
      v[50] = INCR("NumK", -1);
     }
 }

//if(v[10]==0)
 //INTERACT("Merde 2", v[6]);
//WRITE("CapitalCapacity",v[10]/v[2]);
//if(v[5]==1 && v[4]>v[40])
 //WRITE("KillMe", 1);
RESULT(v[10] )

EQUATION("FDKCapacity")
/*
Actual capital capacity for the final demand firm
*/

v[0] = V("CapitalLife");
v[1] = V("CapitalProdEndLife");
v[7] = V("CapitalIntens");
v[3] = V("K")/v[7];
v[4] = min(V("KAge"),v[0]);

v[2]=(1-v[1])/v[0];
v[5]=1-v[4]*v[2]; //remaining share of capital
v[6]=v[3]*v[5];//computes the actual stock of this capital vintage that can be used

RESULT(v[6] )




EQUATION("betaEff")
/*
compute the value of betaEff depending on the relative changes of energy price and minimum wage.
*/

v[0]=VL("betaEff",1);
v[1]=V("psi");
v[2]=VL("PriceEN",1);
v[3]=V("PriceEN");
v[4]=VL("MinWage",1);
v[5]=V("MinWage");
v[6]=v[3]/v[2]-1; //compute the growth rate of PriceEN
v[7]=v[5]/v[4]-1; //compute the growth rate of MinWage
v[10]=v[0]*(1+(v[6]-v[7])*v[1]);

if(t>2)
{v[11]=v[10];}
else
{v[11]=v[0];}

RESULT(v[11] )

EQUATION("betaProd")
/*
compute the value of betaEff depending on the relative changes of energy price and minimum wage.
*/

v[0]=V("betaEff");
v[1]=V("betaPrice");
v[2]=V("betaTime");

v[10]=1-v[0]-v[1]-v[2];
RESULT(v[10] )

EQUATION("MoAvGreenInvestment")
/*
Comment
*/

v[0]=V("GreenInvestment");
v[1]=VL("GreenInvestment",1);

v[2]= 0.9*v[1]+0.1*v[0];

RESULT(v[2] )


EQUATION("MoAvGreenInvestmentGr")
/*
Comment
*/
v[0]=V("MoAvGreenInvestment");
v[1]=VL("MoAvGreenInvestment",1);
if(v[1]==0)
	v[1]=0.0001;

v[2]=v[0]/v[1]-1;

RESULT(v[2] )


EQUATION("MoAvInvestment")
/*
Comment
*/
v[0]=0;
CYCLE(cur, "KFirm")
	v[0] += VS(cur, "KProductionFlow");

v[1]=VL("MoAvInvestment",1);

v[2]= 0.9*v[1]+0.1*v[0];  // MoAvInvestment

RESULT(v[2] )


EQUATION("MoAvInvestmentGr")
/*
Comment
*/

v[0]=V("MoAvInvestment");
v[1]=VL("MoAvInvestment",1);
if(v[1]==0)
	v[1]=0.0001;

v[3]=v[0]/v[1]-1;

RESULT(v[3] )

EQUATION("MoAvMinWageGr")
/*
Comment
*/

v[0]=VL("MinWage",1);
v[1]=V("MinWage");
v[2]=v[1]/v[0]-1;  // MinWage growth rate

v[3]=VL("MoAvMinWageGr",1);

// Compute moving average
v[4]= 0.99*v[3]+0.11*v[2];  // MoAvMinWageGr

RESULT(v[4] )




EQUATION("KNbrGreenKProd")
/*
Defines the number of engineers working on solar panel productivity
*/
v[0]=VL("KNbrEngineers",1);
v[1]=V("KShareGreenKProd");
v[2]=v[0]*v[1];

RESULT(v[2] )


EQUATION("GreenProductivity")
/*
Changes in the productivity of solar panel thanks to R&D
*/

v[0]=VL("GreenProductivity",1);
v[1]=V("KNbrGreenKProd");
v[2]=V("z");
v[3]=V("GreenKProdShock");
v[4]=-(v[1]*v[2]);
v[5]=1-exp(v[4]);//Proba to innovate increases with the nbr of engineers
//first random draw to define the outcome of R&D i.e. success or failure
if (RND<v[5])
	{
	v[6]=norm(0,v[3]);//second stochastic variable, defines the level of productivity out of the successful R&D process
	}
else
	{
	v[6]=0;
	}
v[7]=max(v[6],0);
v[8]=v[0] + v[7];

RESULT(v[8] )

EQUATION("GreenVintageEN")
/*
compute green energy produced by a green vintage
*/

v[0]=V("GreenK");
v[1]=V("GreenVintageProductivity");
v[2]=V("GreenKAge");
v[3]=V("GreenCapitalDep");
v[4]=V("alpha");
v[5]=v[1]*v[4]*v[0]*pow((1-v[3]),v[2]);
if(v[5]==0)
	v[5]=0.0001; // avoid division by zero

RESULT(v[5] )




EQUATION("GreenEnergyCost")  // Called by KCapital
/*
Compute the investment cost to produce one unit of green energy during the whole life of the vintage.
*/

cur1=SEARCHS(p->up,"KFirm");

v[0]=VS(cur1,"KPrice");

// INTERACT("KPrice",v[0]);

v[1]=V("MaxENProduction");

// INTERACT("MaxENProduction",v[1]);

v[2]=v[0]/v[1];

RESULT(v[2] )



EQUATION("TotLocalGreenInvestment")
/*
Total amount of local green investment
*/

v[0]=SUM("LocalGreenInvestment");

RESULT(v[0] )





EQUATION("GreenInvestment")
/*
Define the amount of green investment
*/
v[2]=0;
if(V("GreenEN")>1.1*V("TotEnergyConsumption"))
  v[4]=0.000*V("GreenEN");
else
	{
	if(V("aGreenInvDirty")==1)
		v[0]=V("DirtyEN");
	else
	{
		//v[0]=V("GreenEN");
		v[0]=V("TotEnergyConsumption");
	}

	v[1]=V("GreenCapacityGrowth");
	v[4]=v[0]*v[1]+0;
	//v[3]=VL("GreenInvestment",1);
	//v[4]= 0.9*v[3]+0.1*v[2];  // MoAvMinWageGrV[3]=
	}
//sprintf(msg, "\n TotEnergyConsumption %lf", V("TotEnergyConsumption") ); plog(msg);
v[5]=VL("GreenInvestment",1);
v[6]=0.9*v[5]+0.1*v[4];
RESULT(v[6] )



EQUATION("InvestmentDecision")
/*
Place an order of K if you need it and did not place an order as yet
*/

//sprintf(msg, "\n InvestmentDecision(%g)", v[0]); plog(msg);
WRITE("InvestmentSpending",0);

v[0]=V("Waiting");
if(v[0]==1)
 END_EQUATION(1); //skip the equation if you already placed an order. To be edited to give the possibility to remove a too late order
//we are here only if there is no pending order

v[20] = VL("Revenues", 1);
v[21] = VL("CapitalCostF", 1);
v[23]=V("maxShareKCostRevenues");
if(v[21]>v[20]*v[23])
  END_EQUATION(0); //DO NOT INVEST IF K SPENDING ALREADY EXCEEDS A SHARE OF REVENUES
v[1]=V("KapitalNeed");
v[2]=V("AvKPrice");

v[10]=0;
v[4] = V("MaxKExpenditureF");
if(v[1]>0 && v[4]>=0 )
 {
  cur = SEARCHS(p->up->up, "Machinery");
  WRITES(cur, "IsEnergyK", 0);
  V("PlaceOrder");
  WRITE("Waiting",1);
  v[10]=1;
 }

v[6]=VL("InvestmentDecision",1);
if(v[6]==1 & v[10]==0)
	v[10]=2;

RESULT( v[10])


EQUATION("KENCapacity")
/*
Actual capital capacity for the energy firm
*/
if(V("KillMe")==1)
 END_EQUATION(0);
v[10]=V("EFType");
if(v[10]==0)
 v[0] = V("MaxPPKAgeBrown");
else
 v[0] = V("MaxPPKAgeGreen");

v[1] = V("ENCapitalProdEndLife");
v[3] = V("InitKENCapacity");
v[4] = min(V("PPKAge"),v[0]);

v[2]=(1-v[1])/v[0];
v[5]=1-v[4]*v[2]; //remaining share of capital
v[6]=v[3]*v[5];//computes the actual stock of this capital vintage that can be used

RESULT(v[6] )



EQUATION("AvUtilization")
/*
Comment
*/
v[0]=V("MaxEnergyCapacity");
v[1] = V("EnergyProductionEF");

if(v[0]>0)
 v[2]=v[1]/v[0];
else
 v[2]=0;
RESULT(v[2] )

EQUATION("OrderedKEN")
/*
Sum of capital ordered by EN firms waiting to be delivered
*/
v[0]=v[1]=0;

CYCLE(cur, "EnergyFirm")
{
 if(VS(cur, "WaitingEF")==1)
  {
   v[0] += VS(cur->hook, "KAmount");
  }
}

RESULT(v[0] )

EQUATION("TimeSinceLastInv")
/*
Time steps since last investment
*/

v[1] = V("WaitingEF");
if(v[1]==1)
 v[2]=0;
else
 v[2] = VL("TimeSinceLastInv", 1)+1;
RESULT(v[2] )

EQUATION("PermissionInv")
/*
Permission to invest, allowed only to the firm with the longest time since last investment and if no other firm is waiting for K
*/
v[4] = VS(c, "WaitingEF");
if(v[4]==-1)
 END_EQUATION(1);
v[1]=-1;
CYCLE(cur, "EnergyFirm")
{
 v[0] = VS(cur, "WaitingEF");
 if(v[0]==1)
  END_EQUATION(0);
 v[2] = VS(cur, "TimeSinceLastInv");
 if(v[2]>v[1])
  {
   v[1]=v[2];
   cur1=cur;
  }

}

END_EQUATION(1);


RESULT(v[3] )

EQUATION("InvestmentDecisionEF")
/*
Place an order of K if you need it and did not place an order as yet
*/
//V("DemographyEN");
v[3] = V("WaitingEF");
if(v[3]==1)
 {
 //sprintf(msg, "\n EF Waiting %lf", v[0] ); plog(msg);
 	END_EQUATION(0); //skip the equation if you already placed an order. To be edited to give the possibility to remove a too late order
 }
//we are here only if there is no pending order

v[0]=t; //INTERACT("InvestmentDecisionEF", v[0]);
v[52] = V("ForcePurchaseKEN");
if(v[52]==1 && V("NumPP")>1)
 INTERACT("Shit InvDecEF", v[52]);
v[50] = V("TimeDeliveryPP");
v[51] = V("FrequencyInvEF");
if(v[0]-v[50]<v[51] && v[52]==0)
  END_EQUATION(2);
WRITE("InvestmentSpendingEF",0); //sprintf(msg, "\n InvestmentDecisionEF(%g)", v[12]); plog(msg);
v[1]=V("SumMaxEnergyCapacity");
v[41] = V("OrderedKEN");
v[2]=V("TotEnergyConsumption");
v[42] = V("maxCapacityProductionEN");
if(v[1]+v[41]>v[2]*v[42] && v[3]!=-1 && v[52]==0)
	{
  //sprintf(msg, "\n MaxKExpenditureEF<=0 %lf", v[7] ); plog(msg);
 	END_EQUATION(3);
 	}
v[50]=V("MaxEnergyCapacity");
v[7] = V("MaxKExpenditureEF");
//if(v[7]<=0 || (v[50]>1 && v[3]!=-1 && v[52]==0))
if(v[7]<=0 )
	{//don't invest if don't have enough money
  //sprintf(msg, "\n MaxKExpenditureEF<=0 %lf", v[7] ); plog(msg);
 	END_EQUATION(4);
 	}

v[30]=V("ExcessEnergyL");
v[11]=V("MaxEnergyCapacity");
v[55]=V("EFType");
if(v[55]==0)
{//brown firm
  if(v[11]!=0)
   { //not new entrant
  		v[9]=V("msENCapacity");
  		v[10]=v[2]*v[42]-v[1];
  		if(v[10]<0 && v[52]==0)
  		 INTERACT("NEG KEN ORDER",v[10]);
  		v[20]=v[9]*v[10];
  		v[12]=V("MaxGrowthEF"); //
  		v[21]=v[11]*v[12];
  		v[22]=min(v[20],v[21]);
  		if(v[10]<0 && v[52]==0)
  		 INTERACT("OPS", v[10]);
   }
  else
   {//new entrant
 	   v[60]=V("TotEnergyConsumption");
     v[61]=V("FirstOrderEFFrac");
 	   //v[22]=v[61]*(v[60]+v[30]);
 	   v[22]=max(v[61]*v[60],v[30]);
   }
}
else
{//green firm
  v[56]=V("StepGreenPremium");
  if(t<v[56])
  {//before transition
    if(v[11]!=0)
     { //not new entrant
    		v[9]=V("msENCapacity");
    		v[10]=v[2]*v[42]-v[1];
    		v[20]=v[9]*v[10];
    		v[12]=V("MaxGrowthEF"); //
    		v[21]=v[11]*v[12];
    		v[22]=min(v[20],v[21]);
    		if(v[10]<0)
    		 INTERACT("OPS", v[10]);
     }
    else
     {//new entrant
      v[60]=V("TotEnergyConsumption");
      v[61]=V("FirstOrderEFFrac");
    	   //v[22]=v[61]*(v[60]+v[30]);
      v[22]=max(v[61]*v[60],v[30]);
     }
   }
   else
   {//after transition
      v[72]=V("TotENCapacity");
      if(v[11]!=0)
       { //not new entrant
      	  v[9]=V("msENCapacity");
      		v[10]=v[2]*v[42]-v[1]+v[72];
      		v[20]=v[9]*v[10];
      		v[12]=V("MaxGrowthEF"); //
      		v[21]=v[11]*v[12];
      		v[22]=min(v[20],v[21]);
      		if(v[10]<0)
      		 INTERACT("OPS", v[10]);
      	}
      else
       {//new entrant
        	v[60]=V("TotEnergyConsumption");
          v[61]=V("FirstOrderEFFrac");
      	  //v[22]=v[61]*(v[60]+v[30]);
      	  v[22]=max(v[61]*v[60],v[30]);
       }
   }
}

	if(v[22]>0 || v[52]==1)
	{
	if(V("EFType")==0)
   v[5] = V("MultiplierCapacityENBrown");
  else
   v[5] = V("MultiplierCapacityENGreen");

	v[3]= v[22]/v[5];
	if(v[52]==1)
	 {
	  v[60] = V("KENCapacity");
	  v[61] = V("MAUtilizationPP");
	  v[3] = v[60]*v[61]/v[5];
    if(V("NumPP")>1)
     INTERACT("Forced KEN purchase error", v[52]);
	 }
  WRITE("KapitalNeedEF", v[3]);
  //sprintf(msg, "\n WRITE KapitalNeedEF %lf", v[3] ); plog(msg);
  cur = SEARCHS(p->up->up, "Machinery");
  WRITES(cur, "IsEnergyK", 1);
  V_CHEATS(cur,"PlaceOrderEF", p);
  WRITE("ForcePurchaseKEN", 0);
	}
RESULT( 5)


EQUATION("PPKAge")
/*
Comment
*/

RESULT(CURRENT+1 )



//DEBUG_AT(0);
/******************************************
*
*	DEMOGRAPHY
*
******************************************/




EQUATION("Health")
/*
Measure the sustainability of the firm comparing the ratio of (smoothed) monetary sales to cumulated profits.
Values close to 0 approach exit, 1 is perfect health.
*/
v[2]=V("aHealth");

v[1]=V("NetWorth");
if(v[1]>0)
 {
 END_EQUATION(v[2]*CURRENT+ (1-v[2]));
 }
else
 END_EQUATION(v[2]*CURRENT);
v[0]=V("smoothMonSales");

v[4]=min(1,v[0]/v[1]);//minimum between 1 and the ration of sales to (neg.) profits

v[5]=v[2]*CURRENT+(1-v[2])*v[4];

//if(v[5]<0)
// INTERACT("neg.health", v[3]);
RESULT(v[5] )

EQUATION("smoothMonSales")
/*
Smoothed value of MonetarySales
*/
V("Trade");
v[0]=V("aMonSales");
v[1]=V("MonetarySales");
RESULT(CURRENT*v[0]+(1-v[0])*v[1] )



EQUATION("Entry")
/*
Entry of new firms, function triggered by SectorEntry
*/
//V("ControlInvestment");

 v[0]=t;
 if(t==1)
  END_EQUATION(0);
//sprintf(msg, "\n Entry(%g)", v[0]); plog(msg);
V("ControlPremiaIncome");
V("Trade");


v[1]=VS(c,"IdGood");
cur1=ADDOBJ("Firm");
WRITES(cur1, "flagNewEntrant", 1);
//INTERACT("New Firm added",v[1]);
cur1->hook=c;
WRITELS(cur1,"Age",0, t);
WRITELS(cur1,"Visibility",V("minVisibility"), t);
WRITES(cur1,"IdFirm",v[20]=V("CounterIdFirm"));
//INTERACT("Entry IdFirm",v[20]);
cur5=ADDOBJS(c,"sFirm");

WRITES(cur5,"IdsFirm",v[20]);
cur5->hook=cur1;
cur1->hook=cur5;

WRITES(cur1,"product",v[1]);
cur=SEARCHS(cur1,"PNeed");
cur2=ADDOBJS(cur,"Ch");
WRITES(cur2,"IdCh",2);
v[2]=VS(c,"FrontierX");
WRITELS(cur2,"x",v[2],t);

cur3=SEARCHS(cur1,"Engineers");
cur4=SEARCH_CND("NumClass",0);

cur5 = ADDOBJS(cur4, "LabClass");
cur5->hook=cur3;
cur3->hook=cur5;
WRITES(cur5, "LCType", 10);

/*
cur3=SEARCH_CNDS(cur1,"IdLabor",1);
cur4=SEARCH_CND("NumClass",1);
cur3->hook=cur4;
cur5 = ADDOBJS(cur4, "LabClass");
WRITES(cur5, "LCType", 1);
cur5->hook=cur3;

cur = ADDOBJS(cur1, "Labor");
WRITES(cur, "IdLabor", 2);
cur4=SEARCH_CND("NumClass",2);
cur->hook=cur4;
cur5 = ADDOBJS(cur4, "LabClass");
WRITES(cur5, "LCType", 1);
cur5->hook=cur;
*/



v[4]=V("MinWage");
//INTERACTS(cur1,"Boh",v[1]); /*********************/

cur2=SEARCHS(cur1,"BankF");
cur3=SEARCHS(p->up,"Bank");
WRITELS(cur2, "CashF", 0,t);

cur4=SEARCHS(cur1,"Capital");
if(cur4==NULL)
	INTERACT("NULLcur4 Entry",v[0]);

cur2->hook=cur3;

WRITES(cur4,"IncProductivity",0);
WRITES(cur4, "PrincipalF", 0);


WRITELS(cur1,"MonetarySales",0, t);

WRITELS(cur1,"backlog",0, t);
v[2] = VS(cur1->hook,"SUnitSales");
v[3] = VS(cur1->hook, "msEntrants");
WRITELLS(cur1,"Q",v[2]*v[3], t,1);
WRITELLS(cur1,"Q",v[2]*v[3], t,2);

WRITELS(cur1,"ExpectedSales",v[2]*v[3], t);
WRITELS(cur1,"DesiredQ",v[2]*v[3], t);
WRITELS(cur1,"CapitalStock",0, t);
WRITELS(cur1,"Ms",0, t);
WRITELS(cur1,"CapitalCapacity",0, t);
WRITELS(cur1,"LaborCapacity",0, t-1);
WRITELS(cur1,"MovAvProfit",0, t);
WRITELS(cur1,"Stocks",0, t);
WRITELS(cur1,"price",1, t);
WRITELS(cur1,"markup",V("maxMarkup"), t);//NEW ENTRANTS START WITH THE MAXIMUM MARKUP

WRITELS(cur1,"backlogSales",0, t);
WRITELS(cur1,"backlogSalesL",0, t);
WRITELS(cur1,"Profit",0, t);
WRITELS(cur1,"MsSector",0, t);
WRITELS(cur1,"NumK",0, t);
WRITELS(cur1,"MovAvExpSales",0, t);
WRITELS(cur1,"LaborForce",0, t);
WRITELS(cur1,"LaborForceL",0, t);
WRITELS(cur1,"UnitDemand",0, t);
WRITELS(cur1,"EN",0, t);
WRITELS(cur1,"LaborGrowth",0, t);
WRITELS(cur1,"UnitSales",0, t);

v[2] = VS(cur1->hook, "newFirmKNeed");
WRITELS(cur1,"KapitalNeed",v[2], t);
WRITELS(cur1,"MaxLaborProductivity",1, t);



WRITELS(cur1,"Vacancies",0, t);
WRITELS(cur1,"MonetarySalesL",1, t);
WRITELS(cur1,"Health",1, t);
WRITELS(cur1,"smoothMonSales",1, t);
WRITELS(cur1,"Visibility",V("minVisibility"), t);
WRITELS(cur1,"SmoothProfit",0, t);
WRITELS(cur1,"RatioVacancies",0, t);

WRITELS(cur4,"IncLearningK",1,t);
WRITELS(cur4,"IncSkillBiais",0.2,t);
WRITES(cur4, "IncProductivity", V("AvCurrentProductivity"));
WRITELS(cur4,"KAge",0,t);

v[50]=V("AvMaxEfficiency");
WRITELS(cur4,"IncEfficiency",0,t);

WRITELS(cur4,"K",0,t);
WRITELS(cur4,"KExpenditures",0,t);

WRITES(cur1, "numBLI", 0);

v[44]=0;
CYCLE_SAFES(cur1, cur, "blItem")
{
	if(v[44]==1)
	DELETE(cur);
	else
	WRITES(cur, "blQ", 0);
	v[44]=1;
}

/*
cur7=SEARCH_CND("NumClass",0);
cur3=SEARCH("Engineers");
cur3->hook=cur7;
cur5 = ADDOBJS(cur7, "LabClass");
cur5->hook=cur3;
WRITES(cur5, "LCType", 10);
*/

//INTERACTS(cur1,"Boh1.5",v[1]); /*********************/

WRITELS(cur1, "CapitalStock", 0, t);//place a fake value with a lag, so that the following line forces the variable to be computed
WRITELS(cur1, "MovAvExpSales", 0, t);
WRITELS(cur1, "ExpectedSales", 0, t);
WRITELS(cur1, "MonetarySales", 0, t);


//INTERACTS(cur1,"Boh2",v[1]); /*********************/

v[22]=VL("MinWage",1);
CYCLES(cur1, cur, "Labor")
     {
      v[14]=VS(cur,"IdLabor");
      cur3=SEARCH_CND("NumClass",v[14]);
      
      cur5 = ADDOBJS(cur3, "LabClass");
      cur->hook=cur5;
      cur5->hook=cur;
      WRITES(cur5, "LCType", 1);
      if(v[14]==1)
       { // compute the first tier workers given their productivity and production needs
        v[2]=VS(cur1,"MaxLaborProductivity");
        v[8]=VS(cur1,"CapitalCapacity");
        v[9]=v[8];
        v[4]=VS(cur1,"DesiredUnusedCapacity");
        v[3]=v[4]*(v[9]/v[2]); // number of workers in the first layer in the first period
        v[8] = VS(cur, "nu");
        v[3]=1/v[8]+1;
        WRITELLS(cur,"NumWorkers",v[3],t,1);
        WRITELS(cur,"NumWorkers",v[3],t);
        v[21]=VS(cur,"wagecoeff");
        v[23]=v[22]*v[21];
        WRITELS(cur,"wage",v[23], t);
       }
      else
       {// when above the first tier workers...
        v[18]=VS(cur,"IdLabor");
        v[76]=v[18]; //Maximum class
        cur3=SEARCH_CND("NumClass",v[18]);
        cur->hook=cur3;
        cur2=SEARCH_CNDS(cur->up,"IdLabor",v[18]-1);
        v[17]=VS(cur2,"nu"); //given the worker ratio between tiers of the tier below
        v[19]=VS(cur2,"NumWorkers"); //and the number of workers in the previous tier
        v[6]=v[19]/v[17]; // compute the required executives
        v[24]=VS(cur,"nu"); // given the worker ration between tiers of the present tier (use different worker ratios in case we want to change it along the hierarchical structure)
        WRITELLS(cur,"NumWorkers",v[6], t,1);
        WRITELS(cur,"NumWorkers",v[6],t);
        v[25]=VLS(cur2,"wage",1);
        v[26]=VS(cur,"wagecoeff");
        v[27]=v[25]*v[26];
        WRITELS(cur,"wage",v[27], t);
        if(v[6]>=v[24] && v[18]>=2) //IMPORTANT: this init expects to be TWO (2) layers of workers, then it creates the additional ones as requested by the initial capital
         {
          cur3=ADDOBJ_EXS(cur->up,"Labor",cur);
          WRITES(cur3,"IdLabor",v[18]+1);
          v[20]=v[6]/v[24];
          WRITELLS(cur3,"NumWorkers",v[20], t,1);
          WRITELS(cur3,"NumWorkers",v[20],t);
          v[28]=v[27]*v[26];
          WRITELS(cur3,"wage",v[28], t);
         }
       }

     }
v[81]=VS(cur1, "LaborCapacity");
//INTERACTS(cur1,"Boh3",v[1]); /*********************/


//INTERACT("Entry",v[0]);
V_CHEAT("PlaceOrder", cur1);
WRITES(cur1, "Waiting", 1);
RESULT( 1)



EQUATION("ClearExitRecord")
/*
Prepare the computation of the exit record
*/


CYCLE(cur, "Sectors")
 {
  WRITES(cur,"numExit",0);
  WRITES(cur,"AvAgeDeath",0);
 }

cur=SEARCH("Bank");
RESULT(1 )

EQUATION("MaxKExpenditureEF")
/*
Maximum costs of purchasing capital for energy firms.
It is computed assuming a flow of free cash available represented by sBalanceF plus a share of the available unused cash for the foreseeable future to finance additional capital interests and repayment.
We approximate the single time repayment cost of borrowing K to be repayed in L periods at interest i as:
R=K [ i / z]

where

z=(1-(1/(1+i)^L))

Consequently, the maximum K the firm can expect to pay is:
K=sBalanceF/[i/z]
*/

v[5] = VL("sCashEF", 1);
v[0] = VL("sProfitEF", 1);
v[1] = V("TimeRepaymentEF");
v[2] = VL("sInterestRate",1);//xxx

v[3]=1-pow(1/(1+v[2]),v[1]);
if(v[5]<0)
 v[5]=0;
if(v[0]<0)
 v[0]=0;
v[4]=(v[0]+v[5]/v[1])/(v[2]/v[3]);

if(V("MaxEnergyCapacity")<=1000) // Allow new firms to invest
	v[4]=9999999999.0;
if(v[4]==0 && V("ForcePurchaseKEN")==1)
 v[4] = V("KExpendituresEN")/2;
RESULT(v[4] )

EQUATION("MaxKExpenditureF")
/*
Maximum costs of purchasing capital.
It is computed assuming a flow of free cash available represented by sBalanceF for the foreseeable future to finance additional capital interests and repayment.
We approximate the single time repayment cost of borrowing K to be repayed in L periods at interest i as:
R=K [ i / z]

where

z=(1-(1/(1+i)^L))

Consequently, the maximum K the firm can expect to pay is:
K=sBalanceF/[i/z]
*/

v[5] = VL("sCashF", 1);
v[0] = VL("sProfit", 1);
v[1] = V("TimeRepayment");
if(v[0]==0) //presumably time t=1
 {
  v[0] = V("ExpectedSales")*V("price")*v[1];//essentially ignore the constraint
 }
v[2] = max(VL("sInterestRate",1),0.0001);//xxx

if(v[5]<0)
 v[5]=0;
if(v[0]<0)
 v[0]=0;

v[3]=1-pow(1/(1+v[2]),v[1]);
v[4]=(v[0]+v[5]/v[1])/(v[2]/v[3]);

RESULT(v[4] )

EQUATION("sCashEF")
/*
Smoothed cash
*/
v[0]=V("CashEF");

v[1]=CURRENT*0.95+0.05*v[0];
RESULT(v[1] )

EQUATION("sCashF")
/*
Smoothed cash
*/
v[0]=V("CashF");

v[1]=CURRENT*0.95+0.05*v[0];
RESULT(v[1] )

EQUATION("LiquidityCashRatio")
/*
Ratio of Sales to cash, used to determine excess cash to dispose of
*/
V("Trade");
v[0] = V("MonetarySales");
v[1] = V("CashF");
if(v[1]<=0)
 v[2]=0;
else
 v[2]=v[0]/v[1];

RESULT(v[2] )



EQUATION("TotLiabilitiesKFV")
/*
*/
v[0]=V("TotLiabilitiesKF");
RESULT(v[0] )

EQUATION("TotLiabilitiesEFV")
/*
*/
v[0]=V("TotLiabilitiesEF");
RESULT(v[0] )

EQUATION("TotLiabilitiesFV")
/*
*/
v[0]=V("TotLiabilitiesF");
RESULT(v[0] )


EQUATION("sTotLiabilitiesKFV")
/*
*/
v[0]=VL("sTotLiabilitiesKFV",1);
v[1]=V("TotLiabilitiesKFV");
v[2]=v[0]*0.95+0.05*v[1];
RESULT(v[2] )


EQUATION("sTotLiabilitiesEFV")
/*
*/
v[0]=VL("sTotLiabilitiesEFV",1);
v[1]=V("TotLiabilitiesEFV");
v[2]=v[0]*0.95+0.05*v[1];
RESULT(v[2] )


EQUATION("sTotLiabilitiesFV")
/*
*/
v[0]=VL("sTotLiabilitiesFV",1);
v[1]=V("TotLiabilitiesFV");
v[2]=v[0]*0.95+0.05*v[1];
RESULT(v[2] )



EQUATION("sBalanceEF")
/*
Smoothed BalanceEF
*/
v[0]=V("BalanceEF");

v[1]=CURRENT*0.95+0.05*v[0];

RESULT(v[1] )


EQUATION("sBalanceK")
/*
Smoothed BalanceEF
*/
v[0]=V("BalanceK");

v[1]=CURRENT*0.95+0.05*v[0];

RESULT(v[1] )




EQUATION("sBalanceF")
/*
Smoothed BalanceF
*/
v[0]=V("BalanceF");

v[1]=CURRENT*0.95+0.05*v[0];

RESULT(v[1] )



EQUATION("Exit")
/*
Remove firms with too poor Health
*/
V("Trade");
//V("ControlInvestment");
 v[0]=t;
//sprintf(msg, "\n Exit(%g)", v[0]); plog(msg);
V("ControlPremiaIncome");
V("ControlLaborCost");
V("ClearExitRecord");
V("MegaCheckF");

WRITE("RemovedOverDraftF", 0);
WRITE("RemovedAssetsF", 0);
WRITE("RemovedDepositsF", 0);
WRITE("CapitalDestroyed", 0);

cur5 = SEARCHS(p->up,"Bank");
WRITES(cur5,"NonPerformingLoansTotal",0);
WRITES(cur5,"NPLF",0);

if(V("NoExit")==1)
 END_EQUATION(0);

v[4]=v[70]=0;

v[18]=V("minAgeExit");
v[19]=V("minRRoKExit");
v[40]=v[41]=v[42]=v[43]=v[44]=0;
CYCLE_SAFE(cur, "Firm")
 {
  v[6]=VS(cur,"Waiting");
  v[16]=VS(cur,"Age");
  v[50] = VS(cur, "DesiredQ");
  if(v[16]>v[18]) //don't kill too young firms
   {
    v[2]=VS(cur->hook,"minHealth");
    v[3]=VS(cur,"Health");
    cur2=SEARCHS(cur,"BankF");
    v[20]=VS(cur2,"DebtF");//xxx
    v[70]+=v[20];
    v[7]=VS(cur,"NetWorth");
    v[17]=VS(cur,"RRoK");
//    if(v[3]<v[2])
//     if(v[7]<0)
    v[27] = VS(cur, "CashF");
    v[28] = VS(cur, "KAge"); //is in K, but it is relevant only of there is 1 single K and it is too old
    v[29] = V("CapitalLife");
    if((v[17]<v[19] && v[27]<0 && VS(cur->hook->up,"NFirmsS")>2)||v[28]>v[29]||v[50]==0)
     {
      if(v[50]==0 && V("ExitFlag")==1)
        INTERACTS(cur, "Exit: DesiredQ", v[50]);
       if(v[6]==1)
        {//firm is waiting an order, to be removed
         v[30] = VS(cur, "IdFirm");
         cur2 = SEARCH_CND("IdClient", v[30]);
         if(cur2==NULL)
          INTERACTS(cur, "Err. exiting waiting firm", v[30]);
         else
          {
           if(INCRS(cur2->up, "NumOrders",-1)==0)
            {
             WRITES(cur2,"KAmount",0);
             WRITES(cur2,"KCompletion",0);
             WRITES(cur2,"TimeWaited",0);
             WRITES(cur2,"Kproductivity",0);
             WRITES(cur2,"KEfficiency",0);
             WRITES(cur2, "IdClient", -1);
             cur2->hook=NULL;
            }
           else
            {
             DELETE(cur2);
            }
          }
        }
       v[4]-=v[27];
      INCRS(cur->hook->up,"NFirmsS",-1);
      v[5]=VS(cur,"Age");
      if(V("ExitFlag")==1 )
        INTERACTS(cur,"Dying", v[7]);
      INCRS(cur->hook->up,"AvAgeDeath",v[5]);
      INCRS(cur->hook->up,"numExit",1);
      v[20]=v[21]=0;
      cur1 = SEARCHS(cur, "Engineers");
      DELETE(cur1->hook);
      /*
      CYCLE_SAFES(cur1, cur2, "LabClass")
      {
      	if(cur2->hook==cur1)
      	  {
      	   DELETE(cur2);
      	   break;
      	  } 
      }
      */
      CYCLES(cur, cur1, "Labor")
      {
      	DELETE(cur1->hook);
      }
      
      CYCLES(cur, cur1, "Capital")
      {
      	v[20]+=VS(cur1, "PrincipalF");
      	v[21]+= VS(cur1, "KExpenditures");
      	
      }
			v[4]+=v[20];
			v[41]+=v[21];
			v[39] = VLS(cur, "CashF", 1);
			if(v[39]<0)
			 v[40]+=v[39];
			else
			 v[42]+=v[39]; 
      DELETE(cur->hook);
      DELETE(cur);
     }
   }
 }
//cur5 = SEARCHS(p->up,"Bank");
INCRS(cur5,"NonPerformingLoansTotal",v[4]);
WRITE("RemovedOverDraftF", v[40]);
WRITE("RemovedAssetsF", v[41]);
WRITE("RemovedDepositsF", v[42]);
WRITES(cur5,"NPLF",v[4]);
RESULT(v[4] )



EQUATION("ControlGlobalEnergyTrade")
/*
Check that energy revenues equal energy costs by consumers 
*/
v[0] = V("SumRevenuesEF");
v[1] = V("SumENCost");
v[2] = V("SumKENCost");
v[3]=v[0]-v[1]-v[2];

if(abs(v[3])/v[0]>0.000001)
  INTERACT("Mismatch energy trading", v[3]);
RESULT(v[3] )

EQUATION("EnergyProductionEF2")
/*
Total energy produced by the firm
*/

V("PriceEN");
v[0]=0;
CYCLE(cur, "CapitalEF")
{
v[0]+=VS(cur, "EnergyProductionPP")+VS(cur,"FairEnergyPP");
}
RESULT(v[0] )

EQUATION("Demography")
/*
Comment
*/
V("ControlPremiaIncome");
V("FinancialTrading");
V("TotalAssets");
V("BankControlCurrent");
V("BankControlCapital");
V("RetainedProfitsBank");


cur=SEARCHS(p->up,"Supply");


VS(cur,"Exit");
v[3]=V("numExit");
//v[5]=INCR("NFirmsS",-v[3]);
//if(v[5]<=0)
 //INTERACT("Removed last firm in sector. Likely crash will follow", v[3]);
if(v[3]>0)
 MULT("AvAgeDeath",1/v[3]);

v[4]=V("probEntry");
if(RND<v[4])
 { //v[1]=VS_CHEAT(cur,"Entry",p);
	 v[1]=V_CHEATS(cur,"Entry",p);
   INCR("NFirmsS",v[1]);
 }
RESULT( 1)




EQUATION("CounterIdFirm")
/*
Issue idFirm progressive numbers
*/

RESULT(CURRENT+1 )


/******************************************
*
*	DEMAND
*
******************************************/


EQUATION("x")
/*
Characteristics' values, constant unless they are a of a specified Id.
*/

v[0]=V("IdCh");
if(v[0]==1)
 v[1]=VLS(p->up->up,"price",1);
else
 {
  v[1]=CURRENT;
 }
RESULT(v[1] )


EQUATION("TotIterations")
/*
Total number of iterations for a class
Thios is computed separately, in order to take into account the redistribution of expenditure due to missing goods, and avoid that expenditure level is modified by rounding the number of iterations to an integer
*/

v[5]=0;
CYCLE(cur, "Need")
 {
  v[1]=VS(cur,"ProdExists");
  if(v[1]==1)
   {
    v[2]=VS(cur,"NumIterations");
    v[3]=VS(cur,"TempIterations");
    v[4]=v[2]+v[3];
    v[5]+=v[4];
   }
 }

RESULT(v[5] )


EQUATION("Visibility")
/*
Comment
*/
v[0]=V("backlog");
v[1]=CURRENT;
//v[2]=V("MonetarySales");
v[2]=V("ExpectedSales");
if(v[2]<1)
 v[2]=1;

v[3]=(v[2]-v[0])/v[2];

v[4]=max(v[3],V("minVisibility"));

//v[5]=v[1]*0.9+0.1*v[4];
v[6]=V("aVisibility");
v[5]=v[1]*(1-v[6])+v[6]*v[4];

v[10] = VL("price", 1);
v[11] = VL("ExtraPrice",1);

v[12]=v[10]/(v[10]+v[11]); 

v[13]=v[1]*v[6]+(1-v[6])*v[12];
RESULT(v[13] )


EQUATION("TTB_multiplWinner")
/*

The changes cannot work, since you get the optimum before making any selection.


Standard TTB, but all the winners remain marking "app" to 1, and returning the number of winners.

Choose one option on the base of their characteristics "x", using "tau" as tolerance and "Delta" as random bias of the observed characteristics' values.

Consumers' preferences are the order by which the characteristics are used to filter out (perceived) inferior products.

For each characteristics, the perceived value for each option is a random value drawn from a normal centered around the true value and variance equal to Delta.

All options scoring more than tau times the max value for the current characteristics are considered equivalent to the maximum, and therefore not filtered away.

Before starting the selection procedure, options are scanned to remove those scoring less than a minimum on some characteristic.
*/

//V("MinimumPriceSet"); // make sure the minimum parameter for the price characteristic has been set
v[0]=0;
v[31]=VS(c->up,"Expenditure")/VS(c->up,"TotIterations"); //amount to spend on this iteration
v[30]=VS(c,"IdNeed");

if(c->hook==NULL)
 {
  cur9=SEARCH_CND("IdGood",v[30]);
  c->hook=cur9;
 }
else
 cur9=c->hook;

CYCLES(c, cur, "DCh")
  WRITES(cur,"temp",-1);//set to -1 the max value

v[72]=1;
while(v[0]==0)
{//repeat for increasing levels of visibility as long no firms have been found
//   CYCLE(cur, "Firm")
 v[50]=0;
 CYCLES(cur9,cur8, "sFirm")
   v[50]+=VLS(cur8->hook,"Visibility",1);
 CYCLES(cur9,cur8, "sFirm")
   {
    v[51]=VLS(cur8->hook,"Visibility",1);
    WRITES(cur8,"app1",v[51]/v[50]);
   }

  CYCLES(cur9,cur8, "sFirm")
  {
   cur=cur8->hook;
   v[24]=1; //assume the option to be viable
   //cur3=SEARCH_CNDS(cur,"IdPNeed",v[30]);
   v[37]=VS(cur,"product");
   if(v[37]!=v[30] || RND>VLS(cur8,"app1",1)*v[72])
    {// if the the firm does not produce the product the consumer is looking for, exclude it from the avaialble options
     WRITES(cur,"app",0);
    }
   else
    { // if the firm produces the required good
     WRITES(cur,"app",1);
     v[0]++;
     CYCLES(cur, cur1, "Ch")
      {//for any characteristic
       v[20]=VS(cur1,"IdCh");
       cur2=SEARCH_CNDS(c,"IdDCh",v[20]); //find the ch. of the option you are browsing
       v[4]=VS(cur2,"Delta"); // observation error in the quality of the good
       v[22]=VS(cur1,"x");
       v[71]=norm(v[22],v[4]*v[22]); //this is the observed value
       //v[71]=norm(v[22],v[4]); //ABSOLUTE VARIANCE
       v[23]=max(0,v[71]);
       WRITES(cur1,"obs_x",v[23]); //write the observed value
      }
    }
  }
 if(v[0]==0)
  {//if no firm managed to get visible push up visibility and try again.
   v[72]++;
  }
  if(v[72]>100)
   INTERACTS(c, "No visible firms", v[72]);
}

//Do a proper choice among the (more than one) viable options

//INTERACT("First", v[0]);
v[80]=v[0];
CYCLES(c, cur, "DCh")
 {//for each characteristic, in the order of the decision maker
  v[27]=1;
  v[1]=VS(cur,"IdDCh");
  v[25]=VS(cur,"NegativeQuality"); // to control for negative value of quality such as pice
  v[3]=VS(cur,"tau"); // the tolerance parameter

  //CYCLE(cur1, "Firm")
  CYCLES(cur9,cur8, "sFirm")
   { //find the maximum
    cur1=cur8->hook;
    v[7]=VS(cur1,"app");
    if(v[7]==1)
    {
     //v[8]=VS(cur1,"curr_x");
     cur3=SEARCH_CNDS(cur1,"IdCh",v[1]);
     v[8]=VS(cur3,"obs_x");
     if(v[27]==1 ||v[6]*v[25]<v[8]*v[25])
       {cur->hook=cur3;
        v[27]=0;
        v[6]=v[8];
       }
     }
   }

  v[74]=100000000;
  //CYCLE(cur1, "Firm")
  CYCLES(cur9,cur8, "sFirm")
   { //second cycle: remove options below maximum * tau
    cur1=cur8->hook;
    v[7]=VS(cur1,"app");
    if(v[7]==1)
    {
     cur3=SEARCH_CNDS(cur1,"IdCh",v[1]);
     v[8]=VS(cur3,"obs_x");
     if(v[25]==-1)
      v[33]=1/v[3];
     else
      v[33]=v[3];
     v[78]=v[8]*v[25];
     v[79]=v[6]*v[33]*v[25];
     v[77]=v[79]-v[78];
     if(v[8]*v[25]<v[6]*v[33]*v[25]-0.00001)//approximations sometime makes things weird
      {//too low value: remove
      if(abs(v[6]-v[8]) < 0.00001)
       INTERACTS(cur3,"Merda1",v[33]);

       WRITES(cur1,"app",-1);
       v[0]--;
      //sprintf(msg, " in need %g of class %g\n", v[30],VS(c,"NumClass"));
      //plog(msg);
      }

    }
   }
//INTERACT("Subsequ", v[0]);
 }
if(v[0]==0)
 INTERACTS(c,"No firms left",v[0]);//error control: v[0] must be >=1

if(v[80]!=v[0])
 {
 //sprintf(msg, " %g in need %g of class %g\n", v[0]/v[80], v[30],VS(c,"NumClass"));
  //plog(msg);
 }
//CYCLE(cur, "Firm")
CYCLES(cur9,cur8, "sFirm")
 {
  v[32]=VS(cur8->hook,"app");
  if(v[32]==1)
   INCRS(cur8->hook,"MonetarySales",v[31]/v[0]);
 }

RESULT(v[0] )



EQUATION("Production")
/*
After trading fix any remaining variable to compute
*/

V("ControlLaborCost");

v[0]=v[1]=v[5]=v[6]=v[7]=v[8]=v[9]=v[70]=0;

cur = SEARCH("Supply");
  CYCLES(cur, cur1, "Firm")
   {
    VS(cur1,"Profit");
    VS(cur1,"InvestmentDecision");
    v[5]+=VS(cur1,"Age");
    v[6]++;
    VS(cur1,"WagePrem");

    v[8]=0;
    CYCLE_SAFES(cur1, cur2, "Labor")
    	{
      v[4]=VS(cur2,"NumWorkers");
      v[8]+=v[4];
      //INCRS(cur2->hook,"tempWage",v[2]*v[4]);
      if(v[4]==0 && VS(cur2,"IdLabor")!=1)
       {
     //INTERACTS(cur2, "Removed Labor", v[4]);
     if(SEARCHS(cur2->hook->up, "LabClass")==cur2->hook)
       {cur2->hook->hook=NULL;
        WRITES(cur2->hook, "LCType", 0);
       }
      else 
       DELETE(cur2->hook);
       DELETE(cur2);
       }
    	}
   v[9]+=v[8];
   //cur5=SEARCHS(cur1,"BankF");
   VS(cur1,"NetWorth");
   cur6 = SEARCHS(cur1, "Engineers");
   VS(cur6, "WageEngineers");
   }
cur1 = SEARCH("Machinery");
CYCLES(cur1, cur, "KFirm")
 {
   cur6 = SEARCHS(cur, "KEngineers");
   VS(cur6, "KWageEngineers");
   v[4]=VS(cur,"KWagePrem");
  CYCLE_SAFES(cur, cur2, "KLabor")
   {
    v[4]=VS(cur2,"KPremia");
    v[3]=VS(cur2,"KNbrWorkers");
    
    if(v[3]==0 && VS(cur2,"IdKLabor")!=1)
     {
     //INTERACTS(cur2, "Removed KLabor", v[4]);
      if(SEARCHS(cur2->hook->up, "LabClass")==cur2->hook)
       {cur2->hook->hook=NULL;
        WRITES(cur2->hook, "LCType", 0);
       }
      else 
       DELETE(cur2->hook);
     DELETE(cur2);
     }
   }
  v[70]+=VS(cur,"DebtK");//xxx
 }

cur = SEARCH("Energy");
CYCLES(cur, cur1, "EnergyFirm")
{
	VS(cur1, "WagePremEF");
}

/*
 v[5]=V("TotPremia");
 v[6]=V("TotWage");
 if(v[5]!=v[0] || v[6]!=v[1])
  {
   INTERACT("Failed income",v[1]);
  }
*/
WRITE("AvAge",v[5]/v[6]);

RESULT(1 )


EQUATION("GovDemHH")
/*
Government demand expressed by the class
*/
v[0] = V("GovDemTot");
v[3] = V("ExtraGovSpending");
v[1] = V("ShareIndividuals");
v[2]=(v[0]+v[3])*v[1];
RESULT(v[2] )

EQUATION("GDGDPRatio")
/*
Gov. demand to GDP ratio
*/

v[0] = V("GovDemTot");
v[3] = V("ExtraGovSpending");
v[1] = V("GDPnom");
v[2]=(v[3]+v[0])/v[1];
RESULT(v[2] )

EQUATION("ExtraGovSpending")
/*
In case public debt is negative a share of is used for extra spending
*/
v[0] = VL("GovernmentFund",1);
v[1] = V("shareExtraGovSpending");
if(v[0]>0)
 v[2]=v[0]*v[1];
else
 v[2]=0; 
RESULT(v[2] )


EQUATION("DeficitGDPRatio")
/*
Gov. demand to GDP ratio
*/

v[0] = -V("GovDeficit");
v[1] = V("GDPnom");
v[2]=v[0]/v[1];
RESULT(v[2] )

EQUATION("GovDemTot")
/*
Total government demand
1) At time t=1 it is a share of GDP
2) If the ratio deficit/GDP is below 3% of GDP demand is increased by twice the GDP rate if positive and half if negative
3) if the ratio deficit/GDP is above 3% of GDP govdemand is increased by 80% of GDP rate if positive and by 20% if negative
*/
v[1] = V("shareGDup");
v[3] = VL("GDPnom",1);
v[2] = V("GDPnom");

if(t==1)
 END_EQUATION(v[2]*v[1]);
v[4] = VL("GovDemTot", 1);
v[5] = VL("GovernmentFund",1);
/*
if(v[5]>0)
 v[50]=v[5]*0.05;
else*/

 v[50]=0; 
v[10] = V("UnemploymentRate");

v[7]=-v[5]/v[3]; //ratio debt/gdp
v[14] = VL("TaxRevenues", 1);
//if(v[7]>10 && t> 100)

v[20] = VL("AvPrice", 1);
v[21] = VL("AvExtraPrice", 1);
if(v[21]>v[20]*5)
 {v[4]+=v[50];
  WRITE("shareGovGDP", v[4]/v[2]);
  END_EQUATION(v[4]);
 }

if(v[10]>0.05)
 {v[11]=(v[10]-0.05)/0.95;
  v[8] = 1+V("increaseGovDemTot")*v[11];
  v[6]=v[4]*v[8];
  v[6]+=v[50];
  WRITE("shareGovGDP", v[6]/v[2]);
  END_EQUATION(v[6]);
 }

v[4]+=v[50];
WRITE("shareGovGDP", v[4]/v[2]);
RESULT(v[4])


EQUATION("shareGDup")
/*
Share of gov demand on GDP increment
Initially high to trigger take off then levels to minimum level
*/
v[0] = VL("shareGDup", 1);
v[1] = V("stepshareGDup");
v[2] = V("minshareGDup");
v[3]=v[0]-v[1];
if(v[3]<v[2])
 v[3]=v[2];
RESULT(v[3] )


EQUATION("Trade")
/*
Set a trading cycle:
- initialize "sales" to zero in firms;
- compute the sales for each firm as the total of classes and needs
*/
v[10]=t;
//sprintf(msg, "\n Trade(%g)", v[10]); plog(msg);

V("CheckLaborClass");


v[40]=0;
CYCLE(cur1, "Supply")
 {
  CYCLES(cur1, cur, "Firm")
  {
   v[1]=VS(cur,"MonetarySales");
   WRITES(cur,"MonetarySalesL",v[1]); // before setting the sales to 0 for the current period computation, register the lagged value of monetary sales
   WRITES(cur,"MonetarySales",0);
   v[41]=VS(cur,"CapitalCapacity");
   v[42] = VLS(cur, "LaborCapacity", 1);
   v[43] = VLS(cur, "price", 1);
   //v[40]+=v[43]*min(v[41],v[42]);
   v[40]+=v[43]*v[41];

  }
 }
 

WRITE("MaxMonetaryCapacity", v[40]);
v[44]=v[45]=0;
CYCLE(cur, "Class")
 {
  v[44]+=VS(cur,"Expenditure");
  v[45]+=VLS(cur, "GovDemHH",1);
 }

//LOG("\n%g \t %g\t%g\t%g", v[40],v[44], v[45], v[44]+v[45]);
v[70]=v[71]=0;
CYCLE(cur, "Class")
 {
  v[4]=VS(cur,"Expenditure")+VLS(cur, "GovDemHH",1);
	 v[70]+=v[4];
  //WRITES(cur,"NoConsumption",0); // after having computed the expenditure set the non expenditure to 0, to be computed again inthis period for the following period expenditures
  v[14]=v[21]=0;
  v[18] = VS(cur, "Competitiveness");

  CYCLES(cur, cur1, "Need")
   { // Cycle through the needs of the class
    if(cur1->hook==NULL)
      {//this hook should point to the sector of the need, find it if not already set
       v[10]=VS(cur1,"IdNeed");
       cur1->hook = SEARCH_CND("IdGood", v[10]);
      }
    if(VS(cur1->hook, "NFirmsS")<1)
     INTERACTS(cur1, "Number of SFirm=0",v[14]);
    v[12]=VS(cur1, "Share");
    v[13]=v[12]*v[4];//share of expenditure on this need
    v[30]=-1;
    CYCLES(cur1->hook, cur2, "sFirm")
     {//Find the maximum price
     if(cur2->hook==NULL)
     		INTERACTS(cur2, "missing hook", v[12]);

      cur3 = SEARCH_CNDS(cur2->hook, "IdCh", 1);
      v[31] = VS(cur3, "x");
      if(v[31]>v[30])
        v[30]=v[31];

     }
    v[30]*=1.1;//increase the maximum price of 10% to avoid the indicator to be 0
    v[17]=0;
    CYCLES(cur1->hook, cur2, "sFirm")
     {//Cycle through all firms producing the good for this need

      if(VS(cur2->hook, "CapitalCapacity")>0)
      {
      cur4 = SEARCHS(cur2->hook, "Ch");
      v[15]=1;
      CYCLES(cur1, cur3, "DCh")
        {//Double cycle, through the characteristics of the firm and those of the consumer class
        	v[14] = VS(cur4, "x"); //value of the characteristic
        	v[16] = VS(cur3, "tau");//preference of the class for this characteristic
        	if(v[14]==0)
        		INTERACTS(cur4,"Zerox", v[14]);

        	if(VS(cur4, "IdCh")==1)
          	v[15]*=pow(v[30]-v[14],v[16]);//Factor for price
         else
          	v[15]*=pow(v[14],v[16]);//Factor for quality
        	cur4=cur4->next;
        }
        v[22] = VLS(cur2->hook, "Visibility",1);
        if(v[22]==0)
        	INTERACTS(cur,"ZeroVisibility", v[22]);

        v[19]=pow(v[15],v[18])*v[22];
        if(isinf(v[19])==true)
          INTERACTS(cur2,"Boh inf", v[15]);
      WRITES(cur2, "app2", v[19]);//write the indicator for this sFirm
      v[17]+=v[19];
      }
     }
    v[40]=v[42]=v[43]=0;
    if(v[17]==0)
       INTERACTS(cur1->hook,"ZeroSales", v[17]);
    v[60] = VS(cur, "NumClass");
    CYCLES(cur1->hook, cur2, "sFirm")
     {//Cycle again through the firms supplying this good
     if(VS(cur2->hook, "CapitalCapacity")>0)
      {
      v[20] = VS(cur2, "app2");
      v[43]+=v[20]/v[17];
      v[21]= v[13]*v[20]/v[17];//Share of the expenditures allocated to the firm
      if(v[21]<0)
       INTERACTS(cur,"NEGSALES", v[21]);
      INCRS(cur2->hook, "MonetarySales", v[21]);
      cur4=NULL;
      CYCLES(cur2->hook, cur5, "ClassSales")
      {
      	v[61] = VS(cur5, "IdCS");
      	if(v[61]==v[60])
      	 {
      	  cur4=cur5;
      	  break;
      	 }
      }
      if(cur4==NULL)
       {cur4 = ADDOBJS(cur2->hook, "ClassSales");
        WRITES(cur4, "IdCS", v[60]);
       }
      if(cur4->hook==NULL)
       cur4->hook=cur;
      WRITES(cur4, "valueCS", v[21]);
      WRITES(cur2, "app3", v[20]/v[17]);
      v[71]+=v[21];
      }
     }
   }
  }

//V("ReturnExtraExpenses");

/**
v[90]=v[70]-v[71];
if(abs(v[90])>0.001)
 {
//sprintf(msg, "\n Trade(%g)", v[90]); plog(msg);
//sprintf(msg, "\n v70(%g)", v[70]); plog(msg);
//sprintf(msg, "\n v71(%g)", v[71]); plog(msg);
 LOG("\nExpenditures %lf\nMonetary sal %lf (%lf)\n",v[70],v[71], abs(v[70]-v[71]));
 INTERACT("Trade",v[90]);
 }
**/
//if(V("ExitFlag")==1)
 //INTERACT("PRE-redistribution", v[0]);
V("Redistribute1");
RESULT( 1)


EQUATION("ReturnExtraExpensesXXX")
/*
For each firm compare the level of monetary sales with production capacity.
In case monetary sales exceeds production, includins some margin, the extra monetary expenses are returned to the consumer classes
*/
CYCLE(cur, "Class")
{
	WRITES(cur, "appRE", 0);
}
v[0]=0;
//INTERACT("BEFORE", v[0]);
CYCLE(cur, "Firm")
{
	v[0] = VS(cur, "MonetarySales");
	v[1]=0;
	CYCLES(cur, cur1, "ClassSales")
		v[1] += VS(cur1, "valueCS");
  if(v[0]>0 && (v[1]-v[0])/v[0]>0.0000001)
   INTERACTS(cur, "Err. Return Extra Expenses", v[1]-v[0]);
  v[2] = VLS(cur, "price", 1);
  v[3]=v[0]/v[2];
  v[4] = VLS(cur, "LaborCapacity",1);
  v[5] = VS(cur, "CapitalCapacity");
  v[6]=min(v[4],v[5])*2;
  if(v[6]<v[3])
   {
    WRITES(cur, "MonetarySales", v[6]*v[2]);
    v[7]=v[0]-v[6]*v[2];
    CYCLES(cur, cur1, "ClassSales")
    {
      v[9] = VS(cur1, "valueCS");
    	v[8]=v[7]*v[9]/v[0];
    	//INCRS(cur1->hook, "CashC", v[8]);
    	INCRS(cur1->hook, "Expenditure", -1*v[8]);
    	INCRS(cur1->hook, "appRE", v[8]);

    }
   }
}
RESULT(1 )

EQUATION("Redistribute1")
/*
After consumers' posted their MonetarySales (in Trade) this equation redistributes demand in excess from firms with too small capacity to those in excess of capacity minimizing the backlogs
*/

v[80] = V("ExtraCapacity");
CYCLE(cur, "Sectors")
{//for each sector independently, so as to ensure that total spending per sector remains constant
	v[0]=v[1]=v[2]=v[8]=0;
	CYCLES(cur, cur1, "sFirm")
	{
	 v[5] = VS(cur1->hook, "MonetarySales");
	 WRITES(cur1->hook, "tMonSales", v[5]);//theoretical mon. sales, used to compute expected sales
	 v[0]+=v[5];
	 v[3] = VLS(cur1->hook, "price",1);
	 WRITES(cur1, "app1", v[5]/v[3]);
	 v[4] = VLS(cur1->hook, "LaborCapacity",1);
	 v[25] = VS(cur1->hook, "CapitalCapacity");
	 v[6]=min(v[4],v[25])*v[80]; //capacity
	 WRITES(cur1, "app2", v[6]);
	 v[2]+=v[6]; //total capacity
	 WRITES(cur1, "app5", VS(cur1,"app3"));//transfer in app5 the content of app3, i.e. the global quality indicator for the firm
	 if(v[5]/v[3]>v[6])
	  {//excess demand
	   v[7]=v[6]*v[3]-v[5];//excess monetary sales
	   WRITES(cur1, "app3", v[7]);
	   v[1]+= -v[7]; //m.sales to reallocate
	  }
	 else
	  { //spare capacity
	   v[7]=v[6]*v[3]-v[5];
	   WRITES(cur1, "app3", v[7]);
	   v[8]+=v[7];//total spare capacity
	  }
	}
	WRITES(cur, "sTotMonSal",v[0]);
	WRITES(cur, "sExcessDemand",v[1]);
	WRITES(cur, "sSpareCapacity",v[8]);

  v[20]=v[19]=min(v[8],v[1]);//redistribute the minimum between spare capacity and excess demand

   v[12]=0;
  SORTS(cur, "sFirm", "app5", "DOWN"); //rank by descending values of quality indicators

  CYCLES(cur, cur1, "sFirm")
  {
   v[9] = VS(cur1, "app3");
  	if(v[9]>0)
  	 {//spare capacity
  	  v[10]=min(v[19],v[9]);
  	  INCRS(cur1->hook, "MonetarySales", v[10]);
  	  WRITES(cur1, "app4", v[10]);
  	  v[19]-=v[10];
  	 }
  	else
  	 {//excess demand
   	  v[11] = VS(cur1, "app3");
   	  v[13]=v[11]*v[20]/v[1];
  	  INCRS(cur1->hook, "MonetarySales", v[13]);
  	 }
  	v[12] += VS(cur1->hook, "MonetarySales");
  	v[70] = VS(cur1->hook, "MonetarySales");
  	v[71] = VS(cur1->hook, "tMonSales");
  	WRITES(cur1->hook, "gapSales", v[70]-v[71]);

  }

  if(abs( (v[0]-v[12])/v[12])>0.01)
   INTERACTS(cur, "Error redistribution", v[12]);
  SORTS(cur, "sFirm", "IdsFirm", "UP");//restore normal ranking

}

RESULT(1 )


EQUATION("TradeXXX")
/*
Set a trading cycle:
- initialize "sales" to zero in firms;
- compute the sales for each firm as the total of classes and needs
*/

CYCLE(cur1, "Supply")
 {
  CYCLES(cur1, cur, "Firm")
  {
   v[1]=VS(cur,"MonetarySales");
   WRITES(cur,"MonetarySalesL",v[1]); // before setting the sales to 0 for the current period computation, register the lagged value of monetary sales
   WRITES(cur,"MonetarySales",0);
  }
 }

v[70]=0;
CYCLE(cur, "Class")
 {
  v[4]=VS(cur,"Expenditure");
  v[70]+=v[4];
  //WRITES(cur,"NoConsumption",0); // after having computed the expenditure set the non expenditure to 0, to be computed again inthis period for the following period expenditures
  v[14]=v[21]=0;
  CYCLES(cur, cur1, "Need")
   { // make a first cycle through needs to check which products are available in the market
    v[10]=VS(cur1,"IdNeed");
    v[12]=0;
    CYCLE(cur2, "Supply")
     {
      CYCLES(cur2, cur3, "Firm")
       { // cycle through all firms
        v[11]=VS(cur3,"product");
        if(v[11]==v[10]) //count how many firms sell the product needed
         {//you just need to know if there is 1 firm, don't waste time
          v[12]++;
          break;
         }
       }

     }
    if(v[12]<1)
     {
      v[13]=VS(cur1,"NumIterations");
      v[14]+=v[13];
      v[20]=VS(cur1,"Share");
      v[21]+=v[20];
      WRITES(cur1,"ProdExists",0); // write that the product for the need does not exists
     }
    else
     WRITES(cur1,"ProdExists",1);

   }
  CYCLES(cur, cur1, "Need")
   { // cycle again to redistribute the iterations to the needs for which a product exists
    v[15]=VS(cur1,"Share"); // use the share
    v[22]=1-v[21];
    v[23]=v[15]/v[22];
    v[16]=v[23]*v[14]; // compute the number of iterations non sde from the other needs
    v[17]=round(v[16]); // round them
    WRITES(cur1,"TempIterations",v[17]);
   }


  CYCLES(cur, cur1, "Need")
   {
    v[19]=VS(cur1,"ProdExists");
    if(v[19]==1)
     {
      v[0]=0;
      //v[5]=VS(cur1,"Share");
      //v[6]=v[5]*v[4];
      v[9]=VS(cur1,"NumIterations");
      v[18]=VS(cur1,"TempIterations");

      for(v[8]=0; v[8]<v[9]+v[18]; v[8]++)
        V_CHEAT("TTB_multiplWinner", cur1);

     }
   }
 }

CYCLE(cur, "Sectors")
 {
 VS(cur,"RedistributeSales");
 }

cur=SEARCH("Bank");
RESULT( 1)

EQUATION("RedistributeSales")
/*
This routine redistributes sales in order to avoid excessive backlogs. The logic is that customers choosing firms that cannot deliver will redirect their expenses to firms with available capacity.

The routine collects total capacity in excess of demand and demand in excess of capacity. The latter is redistributed in proportion to the former.

In case total capacity of the industry is not sufficient to meet the demand, the residual is distributed in proportion to sales
*/

if(VS(p->up,"Redistribute")==0)
 END_EQUATION(0);

v[0]=v[1]=v[2]=v[3]=v[8]=v[10]=v[20]=v[21]=v[32]=0;

CYCLE(cur, "sFirm")
 {
  v[4]=VS(cur->hook,"MonetarySales");
  v[8]+=v[4]; //total sales
  v[5]=VLS(cur->hook,"price",1);
  v[6]=v[4]/v[5]; //unit sold
  v[7]=VLS(cur->hook,"Q",1);
  if(v[6]>v[7])
   {//insufficient capacity
    v[0]+=v[4]-v[7]*v[5]; //excess sales over production
    v[20]+=v[7]*v[5];  //sales at full capacity
    //WRITES(cur,"app3",0);
    //WRITES(cur,"app4",v[7]*v[5]);
   }
  else
   {//sufficient capacity
    v[1]+=v[4]; //fully served sales
    v[2]+=v[7]*v[5]-v[4];//excess capacity available to serve extra customers
    v[21]+=VLS(cur->hook,"Q",1);
    v[3]++;
    //WRITES(cur,"app3",1);
    //WRITES(cur,"app4",v[7]*v[5]);
   }
 }
if(v[3]==0)
 END_EQUATION(-1);
v[9]=max(0,v[0]-v[2]); //over sales
v[13]=v[0]-v[9];
CYCLE(cur, "sFirm")
 {
  v[4]=VS(cur->hook,"MonetarySales");
  v[5]=VLS(cur->hook,"price",1);
  v[6]=v[4]/v[5]; //unit sold
  v[7]=VLS(cur->hook,"Q",1);
  if(v[6]>v[7])
   {//oversold
    v[11]=v[7]*v[5];//maximum sales
    //WRITES(cur,"app1",v[11]);
    v[12]=v[9]*v[4]/v[8];
    //WRITES(cur,"app2",v[12]);
   }
  else
   {//undersold
    v[14]=v[7]*v[5]-v[4];
    if(v[2]>0)
      v[11]=v[4]+v[13]*v[14]/v[2];
    else
      v[11]=v[4]+v[13]*VLS(cur->hook,"Q",1)/v[21];
    //WRITES(cur,"app1",v[11]);
    v[12]=v[9]*v[4]/v[8];
    //WRITES(cur,"app2",v[12]);
   }

  v[10]+=v[11]+v[12];
  v[32]+=v[12];

  v[50]=v[11]+v[12];

  WRITES(cur->hook,"MonetarySales",v[50]);
 }

RESULT((v[8]-v[10]) )





EQUATION("LTGDP")
/*
Long term GDP
*/
v[0] = VL("LTGDP", 1);
v[1] = V("GDP");
v[2] = V("aLTGDP");
v[3]=v[0]*v[2]+(1-v[2])*v[1];
if(t==1)
 v[3]=v[1];
RESULT(v[3] )

EQUATION("LTTotProfit")
/*
Long term average profit
*/
V("AggProductivity"); //ensure that AvProfit is computed
v[0] = VL("LTTotProfit", 1);
v[1] = V("TotProfit");
v[2] = V("aLTProfit");
v[3]=v[0]*v[2]+(1-v[2])*v[1];
if(t==1)
 v[3]=v[1];
RESULT(v[3] )

EQUATION("RatioProfitGDP")
/*
Short term average profit
*/
v[0] = VL("LTTotProfit",1);
v[1] = VL("LTGDP",1);
v[3]=v[0]/v[1];
RESULT(v[3] )

EQUATION("markupXXX")
/*
Global version of markup
If average profits tend to fall the markup increases and viceversa
*/
v[0] = V("RatioProfitGDP");
v[1] = VL("RatioProfitGDP",1);

v[3]=v[1]/v[0];
v[6]= v[2] = VL("markup", 1)*v[3];
v[4] = V("maxMarkup");
v[5] = V("minMarkup");
if(v[2]<v[5])
 v[2]=v[5];
if(v[2]>v[4])
 v[2]=v[4];

RESULT(v[2] )

EQUATION("UnitSalesL")
/*
Shifted UnitSales
*/

v[0] = VL("UnitSales",1);
v[1] = VL("UnitSalesL", 1);
if(t==1)
 v[2]=v[0];
else
 v[2]=v[1]*0.95+0.05*v[0];


RESULT(v[2] )

EQUATION("markup")
/*
Individual version of markup
If sales are increasing push up the markup
*/
v[0] = V("UnitSalesL");
v[1] = VL("UnitSalesL",1);
v[3] = V("aMarkupFD");
v[6]= VL("markup", 1);
v[4] = V("maxMarkup");
v[5] = V("minMarkup");
if(t==1)
 {
  v[7]=(v[4]+v[5])/2;
  END_EQUATION(v[7]);
 }
if(v[0]<v[1])
 {//decreasing sales
 v[2]=v[6]*v[3] + (1-v[3])*v[5];
 }
else
 {//increasing sales
 v[2]=v[6]*v[3] + (1-v[3])*v[4];
 }


RESULT(v[2] )


EQUATION("DesiredQ")
/*
Production, as a function of the difference between past stocks and desired ones
*/
V("Trade");
v[0]=VL("Stocks",1)*0.1;
v[1]=V("DesiredStocks"); // percentage of expected sales firms want to stock to face unexpected demand
v[2]=VL("backlog",1);
v[4]=V("ExpectedSales");
v[3]=v[1]*v[4];
v[6]=max(0,v[3]+v[2]+v[4]-v[0] );

RESULT(v[6])


EQUATION("ExpectedSales")
/*
Smoothed level of expected sales, based on theoretical demand pre-distribution
*/
V("Trade");
v[0]=VL("ExpectedSales",1);
v[4]=VL("price",1);
v[1]=V("MonetarySales")/v[4];
//v[1]=VL("UnitSales",1);
v[2]=V("aES");
v[3]=v[0]*v[2]+(1-v[2])*v[1];

RESULT(v[3] )

EQUATION("FeasibleDesiredQ")
/*
Comment
*/

v[6] = V("ExtraCapacity");
v[0]=V("DesiredQ");
v[1]=V("LaborCapacity")*v[6];
v[2]=min(v[0],v[1]);
v[3]=V("CapitalCapacity")*v[6];
v[5]=min(v[3],v[2]);

RESULT(v[5] )


EQUATION("Q")
/*
Actual production, which is the minimum between desired production and constraints
*/


v[16] = VL("Q", 1);
v[6] = V("ExtraCapacity");
v[0]=V("DesiredQ");
v[10] = V("aSadapt");
v[17]=v[7]=sadapt(p, "FeasibleDesiredQ", "Q", v[10]);
//if(v[7]<0)
	//INTERACT("neg. Q",v[7]);

END_EQUATION(max(10,v[7]));
if(v[7]<v[16]*.1)
 v[7]=v[16]*.1;
v[1]=V("LaborCapacity")*v[6];
v[2]=min(v[7],v[1]);
v[3]=V("CapitalCapacity")*v[6];
v[5]=min(v[3],v[2]);
if(v[5]<0)
	INTERACT("neg. Q",v[5]);

RESULT(v[5] )


EQUATION("LaborCapacity")
/*
Maximum capacity allowed by currently employed workers on current capital stock
*/
v[0]=0;
v[1]=V("MaxLaborProductivity");
v[2]=VL("NumWorkers",0); // refers only to first layer workers, not to executives
v[4]=v[2]*v[1];

RESULT(v[4] )



EQUATION("TotNumWorkers")
/*
*/

v[0]=0;
CYCLE(cur, "Firm")
{
	CYCLES(cur, cur1, "Labor")
		v[0] += VS(cur1, "NumWorkers");
}
RESULT(v[0] )

EQUATION("NumWorkersGr")
/*
*/
v[0]=VL("NumWorkers",1);
if(v[0]==0)
	END_EQUATION(0);

v[1]=V("NumWorkers");

RESULT(v[1]/v[0]-1 )



EQUATION("NumWorkers")
/*
Number of workers, computed as the 120% of the workers required to fill expected sales
*/

v[15]=0;
CYCLES(p->up, cur, "Labor")
 { //check how many tiers already exist
  v[15]++;
 }
v[14]=V("IdLabor");
if(v[14]==1)
 { // compute the first tier workers given their productivity and production needs
  //V("aNWDynamic");
  v[0]=VL("NumWorkers",1);
  //v[1]=V("ExpectedSales");
  v[61]=V("DesiredQ");
  v[51] = V("ExpectedSales");
  v[1]=max(v[61],v[51]/2);
  //v[10]=V("backlog");
  //v[11]=v[1]+v[10];
  v[2]=V("MaxLaborProductivity");
  if(v[2]==0)
  	END_EQUATION(v[0]);


  v[30] = V("ExtraCapacity");
  v[8]=V("CapitalCapacity")*v[30];
  v[9]=min(v[1],v[8]);
  v[4]=V("DesiredUnusedCapacity");
  v[3]=v[4]*(v[9]/v[2]);
  v[5]=V("aNW");
  if(v[3]<v[0])
   v[5]=0.9;
   //v[5]=min(0.9, v[5]*2); //falling employment is slower than increasing
  v[6]=v[0]*v[5]+(1-v[5])*v[3]; // number of workers in the first layer
  v[26] = VL("UnemploymentRate",1);
  v[28]=v[6]-v[0];
  if(v[28]>0)
   {//if increasing
     v[28]*=v[26];
     v[6]=v[0]+v[28];
   }

/*
    if(v[6]/v[0]-1 > 1+V("NumWorkersMaxGr") )
  	{
  	if(v[6]-v[0] > V("NumWorkersMinInc") )
  		{
  		v[6]=1+V("NumWorkersMaxGr");
  		}
  	}
*/

  v[33]=(v[3]>v[6])?(v[3]-v[6]):0; //number of vacancies for the first layer, if there are more desired workers than actual new hires
  WRITES(p->up,"Vacancies",v[33]);
  if(v[6]>0)
   {
  	v[54]=v[33]/v[6];//ratio of vacancies to actual workers
   }
  else
   {
    v[54]=0;
   }
  WRITES(p->up,"RatioVacancies",v[54]);
 }

else
 {// when above the first tier workers...
  v[18]=V("IdLabor");
  cur=SEARCH_CNDS(p->up,"IdLabor",v[18]-1);
  v[23]=VS(p->up,"IdFirm");
  v[24]=VS(cur->up,"IdFirm");
  if(v[23]!=v[24])
   v[25]=INTERACT("the firm ID is different from the current firm", v[24]);
  v[21]=VS(cur,"nu"); //given the worker ratio between tiers (defined by the tier below)
  v[19]=VS(cur,"NumWorkers"); //and the number of workers in the previous tier
  v[6]=v[19]/v[21]; // compute the required executives for the current tier
  v[54] = V("RatioVacancies");
  INCRS(p->up,"Vacancies",v[54]*v[6]);

  v[17]=V("nu");
  if(v[6]>=v[17] && v[18]==v[15] && V("FreezeEmployment")==0)
   { // if they are above the workers ratio of this tier and this is the last tier, create a new working class
    cur1=ADDOBJ_EXS(p->up,"Labor",p);
    WRITES(cur1,"IdLabor",v[18]+1);
    v[20]=v[6]/v[17];
    WRITELS(cur1,"NumWorkers",v[20], t);
    WRITELLS(cur1,"NumWorkers",0, t,1); // write also that the number of workers in the previous period is equal to 0, as it is used in the inequality statistics
    v[22]=0;
    CYCLES(p->up->up->up,cur2, "Class")
     {
      if(v[22]==0)
       {
        cur7=SEARCHS(cur2,"BankC");
        cur7=cur7->hook;
       }
      v[22]++;
     }

    if(v[22]-1<v[18]+1 )
     { //starting from the second class (the first are engineers), if it does not exist a class that represnt the new layer of executives, create it
      cur2=SEARCH_CNDS(p->up->up->up,"NumClass",v[18]);
      cur3=ADDOBJ_EXS(cur2->up,"Class", cur2);
      v[31]=0;
      CYCLE_SAFES(cur3, cur4, "LabClass")
      {
       if(v[31]==0)
        {
         v[31]=1;
         WRITES(cur4, "LCType", 0);
        }
       else
        DELETE(cur4); 
      }      
      //cur3=ADDOBJS(cur2->up,"Class");
      //INTERACTS(cur3,"New class",v[22]);
      cur8=SEARCHS(cur3,"BankC");
      WRITELS(cur8,"BalanceC",0, t-1);
      WRITELS(cur8,"OverdraftPaymentC",0, t-1);

      cur8->hook=cur7;
      
      cur5 = SEARCHS(cur3, "LabClass");
      cur1->hook=cur5;
      cur5->hook=cur1;
      WRITES(cur5, "LCType", 1);

      v[44]=VS(cur2,"SavingRate");
      v[45]=VS(cur2->up,"SRMultiplier");
      v[47] = VS(cur2->up, "TopSavingRate");
      v[46]=v[44]*v[45]+(1-v[45])*v[47];
      WRITES(cur3,"SavingRate",v[46]);
//      INTERACTS(cur3,"Inspect SavingRate", v[46]);

      WRITES(cur3,"NumClass",v[18]+1);
      WRITELLS(cur3,"Expenditure",0, t-1,1);
      WRITELS(cur3,"Expenditure",0, t);
      WRITELS(cur3,"Income",0, t-1);
      WRITELS(cur3,"WageIncome",0, t-1);
      WRITELS(cur3,"PremiaIncome",0, t-1);      
      WRITELS(cur3,"Consumption",0, t-1);
      WRITELS(cur3,"GovDemHH",0, t-1);
      WRITELS(cur3,"ShareWageIncome",0, t-1);
      WRITELS(cur3,"SharePremiaIncome",0, t-1);
      WRITELS(cur3,"BankProfitIncome",0, t-1);      
      WRITELS(cur3, "CashC", 0, t-1);
      WRITELS(cur3,"ShareIncome",0, t-1); // reset the share income to be recomputed
      WRITELS(cur3,"Individuals",0,t-1); // set the number of individuals to nu;ber of workers of the new class
      v[35]=VS(cur2,"LorenzInd");
      WRITES(cur3,"LorenzInd",v[35]+v[20]); // set total number of workers as previous total plus new workers
      //WRITELS(cur3,"NoConsumption",0, t-1); // set the savongs due to unavailability of the good to 0
      //INTERACTS(cur3,"Created class",v[44]);
      CYCLES(cur3, cur, "Need")
       { // enter in neds and characterisitcs to change the tau parameter (the minimum is set every period)
        v[27]=VS(cur,"IdNeed");
        CYCLES(cur, cur1, "DCh")
         {
          v[26]=VS(cur1,"IdDCh");
          CYCLES(cur2, cur4, "Need")
           { // cycle among the needs of the preceding class
            v[28]=VS(cur4,"IdNeed");
            if(v[28]==v[27])
             { // when in the same need as the one the new class is cycling cycle through the char of the preceding class
              CYCLES(cur4, cur5, "DCh")
               {
                v[29]=VS(cur5,"IdDCh");
                if(v[29]==v[26])// when in the same characteristic the new class is cycling
                 v[30]=VS(cur5,"tau"); // read the value of the tau parameter
               }

             }
           }
          v[31]=VS(cur1,"tauMultiplier"); // the speed of adjustment of the tolerance level from one class to the following one
          v[34]=VS(cur1,"NegativeQuality");
          if(v[34]>0)
           v[33]=VS(p->up->up->up,"tauMax"); // the asympthotic level of the tolerance level (1 for qualities and 0 for price)
          if(v[34]<0)
           v[33]=VS(p->up->up->up,"tauMin"); // the asympthotic level of the tolerance level (1 for qualities and 0 for price)
          v[32]=v[30]*(1-v[31])+v[31]*v[33]; // adjustment in the treshold level of tolerance
          WRITES(cur1,"tau",v[32]); // finally write the tau for the new consumer class in each of its characteristic for each need
         }

       }
     v[35]=VS(cur3,"ComputeShare"); // set the distribution of expenditure shares across needs for the new class
     //INTERACTS(cur3,"NewClass END", v[35]);
     }
    else
     {
      cur3=SEARCH_CND("NumClass",v[18]+1);
      
      cur5 = ADDOBJS(cur3, "LabClass");
      cur1->hook=cur5;
      cur5->hook=cur1;
      WRITES(cur5, "LCType", 1);
     }
   
   }
  if(v[18]>2 && v[19]<v[21])
   v[6]=0;
 }


RESULT(v[6] )

EQUATION("NumActiveOrders")
/*
Comment
*/
V("Production");
V("PriceEN");

v[0]=0;
CYCLE(cur, "KFirm")
{
	v[0] += VS(cur, "NumOrders");
}

RESULT(v[0] )


EQUATION("MinimumPriceSet")
/*
System equation that mantains the `minimum' parameter for the price characteristic, of the first tyer working class and engineers, above the minimum price among firms (times the variance of the consumer percetion error).
It also sets the Minimum parameter across needs for the price
*/


v[4]=1000000;
CYCLE(cur, "Supply")
 {
  CYCLES(cur, cur1, "Firm")
   {
    v[3]=VLS(cur1,"price",1);
    if(v[3]<v[4])
     v[4]=v[3];
   }

 }
v[10]=V("AvWagecoeff");
CYCLE(cur, "Demand")
 {
  CYCLES(cur, cur1, "Class")
   {
    v[1]=VS(cur1,"NumClass");
    if(v[1]<=1)
     { // start with the engineers and the first tier working class (they both set their minimum independently)
      CYCLES(cur1, cur2, "Need")
       {
        CYCLES(cur2, cur3, "DCh")
         {
          v[2]=VS(cur3,"IdDCh");
          if(v[2]==1)
           { // maintain the minimum above the minimum of the characteritic (price) only for the fist (price) characteritic
            v[5]=VS(cur3,"Minimum"); // check the initialised value of the minimum
            v[6]=VS(cur3,"Delta"); // variance of the distribution of the perceived characteritic around the actaul value
            v[7]=v[6]*v[4];
            v[8]=max(v[5],v[7]); // use either the initially set minimum or the price of the cheapest firm
            WRITES(cur3,"Minimum",v[8]);
           }
         }

       }

     }
    if(v[1]>1)
     { // layers above the first one (different types of executives)
      CYCLES(cur1, cur2, "Need")
       {
        CYCLES(cur2, cur3, "DCh")
         {
          v[9]=VS(cur3,"IdDCh");
          if(v[9]==1)
           {
            cur4=SEARCH_CNDS(cur,"NumClass",v[1]-1);
            CYCLES(cur4, cur5, "DCh")
             { // cycle the DCh for any need (we assume that the minimum for price is equal across needs)
              v[11]=VS(cur5,"IdDCh");
              if(v[11]==1)
               v[12]=VS(cur5,"Minimum"); // read the minimum par of the previsous class for char price
             }
            v[8]=v[12]*v[10];
            WRITES(cur3,"Minimum",v[8]); // write the value of the parameter, as the previous Minimum times the multiplier
           }
         }
       }
     }
   }
 }


RESULT(v[8] )


EQUATION("AvWagecoeff")
/*
Average of the wage multiplier, in case they are different between firms (or capital and final good firms), to be used as a multiplier of the `minimum' parameter for consumers classes of executives
*/

v[3]=v[4]=0;
CYCLE(cur, "Supply")
 {
  CYCLES(cur, cur1, "Firm")
   {
    CYCLES(cur1, cur2, "Labor")
     {
      v[1]=VS(cur2,"IdLabor");
      if(v[1]>1)
       {
        v[2]=VS(cur2,"wagecoeff");
        v[3]+=v[2];
        v[4]++;
       }
     }

   }

 }
CYCLE(cur, "Machinery")
 {
  CYCLES(cur, cur1, "KFirm")
   {
    CYCLES(cur1, cur2, "KLabor")
     {
      v[5]=VS(cur2,"IdKLabor");
      if(v[5]>1)
       {
        v[6]=VS(cur2,"KWagecoeff");
        v[3]+=v[6];
        v[4]++;
       }
     }
   }
 }
v[7]=v[3]/v[4];

RESULT(v[7] )











EQUATION("IncomeCapita")
/*
Comment
*/
v[0]=V("Income");
v[1]=V("Individuals");
if(v[1]>0.001)
 v[2]=v[0]/v[1];
else
 v[2]=0;
RESULT(v[2] )

EQUATION("TotIncomeCapita")
/*
Comment
*/
v[0]=V("TotIncome");
v[1]=V("TotIndividuals");
RESULT(v[0]/v[1] )

EQUATION("TotIncomeCapitaReal")
/*
Comment
*/
v[0]=V("TotIncomeCapita");
v[1]=V("CPI");
RESULT(v[0]/v[1] )

EQUATION("GDPCapita")
/*
Comment
*/
v[0]=V("GDP");
v[1]=V("TotIndividuals");
if(v[1]>0.001)
 v[2]=v[0]/v[1];
else
 v[2]=0;
RESULT(v[2] )


EQUATION("TotConsumption")
/*
Total desired Consumption
*/

RESULT(SUM("Consumption") )

EQUATION("TotDConsConst")
/*
Total desired consumption at constant prices
*/

v[1]=V("TotConsumption");
v[2]=V("CPI");

RESULT(v[1]/v[2] )


EQUATION("TotConsConst")
/*
Total Consumption at constant prices
*/

v[1]=V("SumExpenditure");
v[2]=V("CPI");

RESULT(v[1]/v[2])

EQUATION("shareConsumption")
/*
Comment
*/
v[0]=V("Consumption");
v[1]=V("TotConsumption");
v[2]=v[0]/v[1];
if(v[1]==0)
	v[2]=0;
RESULT(v[2] )

EQUATION("TotIncome")
/*
Total income from the wage
*/

v[1]=V("TotWage");
v[2]=V("TotPremia");

v[3]=0;
CYCLE(cur, "Class")
{
	v[3] += VS(cur, "Income");
}

RESULT(v[3] )

EQUATION("TotWage")
/*
Total wage
*/

v[0]=0;

CYCLE(cur, "Class")
 {
  v[0]+=VS(cur,"WageIncome");
 }

RESULT(v[0] )

EQUATION("AvWage")
/*
Average wage
*/

v[1]=V("TotWage");
v[2]=V("TotIndividuals");

if(v[1]==0 || v[2]==0)
	END_EQUATION(0);

RESULT(v[1]/v[2] )


EQUATION("TotPremia")
/*
Total premia
*/

//V("Production");
v[0]=0;

CYCLE(cur, "Class")
 {
  v[0]+=VS(cur,"PremiaIncome");
 }

RESULT(v[0] )

EQUATION("PayTimeXXX")
/*
Semaphore resetting the parameters collecting the incomes from employees before being computed
*/
V("Trade");
CYCLE(cur, "Class")
 {
  WRITES(cur,"Individuals",0);
  WRITES(cur,"WageIncome",0);
  WRITES(cur,"PremiaIncome",0);
  WRITES(cur,"tempWage",0);
 }

RESULT(1)

EQUATION("WageIncome")
/*
Sum of all wages by workers in the consumer class
*/
v[0]=0;
CYCLE(cur, "LabClass")
{
  v[1] = VS(cur, "LCType");
  switch((int)v[1])
   {
    case 0: break;//token object not used
    case 1: v[2] = VS(cur->hook, "NumWorkers");
            v[3] = VS(cur->hook, "wage");
            v[0]+=v[2]*v[3];
            break;
            
    case 10: v[2] = VS(cur->hook, "RdExpenditure");
            v[0]+=v[2];
            break;
 
    case 2: v[2] = VS(cur->hook, "KNbrWorkers");
            v[3] = VS(cur->hook, "KWage");
            v[0]+=v[2]*v[3];
            break;
            
            
    case 20: v[2] = VS(cur->hook, "KNbrEngineers");
            v[3] = VS(cur->hook, "KWageEngineers");            
            v[0]+=v[2]*v[3];
            break;
            
    case 3: 
//            if(t==17)
  //           INTERACTS(cur, "Merda WageIncome", v[1]);
            v[2] = VS(cur->hook, "NumWorkersEF");
            v[3] = VS(cur->hook, "wageEF");
            v[0]+=v[2]*v[3];
            break;
  
    default:
            INTERACTS(cur, "Error WageIncome", v[1]);
            break;       
   }              
   
}

RESULT(v[0] )



EQUATION("Individuals")
/*
Sum of all wages by workers in the consumer class
*/
v[0]=0;
CYCLE(cur, "LabClass")
{
  v[1] = VS(cur, "LCType");
  switch((int)v[1])
   {
    case 0: break;//token object not used
    case 1: v[2] = VS(cur->hook, "NumWorkers");
            v[0]+=v[2];
            break;
            
    case 10: v[2] = VS(cur->hook, "RdExpenditure");
            v[3] = VS(cur->hook, "WageEngineers");
            v[0]+=v[2]/v[3];
            break;
 
    case 2: v[2] = VS(cur->hook, "KNbrWorkers");
            v[0]+=v[2];
            break;
            
            
    case 20: v[2] = VS(cur->hook, "KNbrEngineers");
            v[0]+=v[2];
            break;
            
    case 3: v[2] = VS(cur->hook, "NumWorkersEF");
            v[0]+=v[2];
            break;
  
    default:
            INTERACTS(cur, "Error Individuals", v[1]);
            break;       
   }              
   
}

RESULT(v[0] )




EQUATION("PremiaIncome")
/*
Sum of all premia given to workers in the consumer class
*/
v[0]=0;
CYCLE(cur, "LabClass")
{
  v[1] = VS(cur, "LCType");
  switch((int)v[1])
   {
    case 0: break;//token object not used

    case 1: VS(cur->hook, "WagePrem"); //ensures that premia are updated
            v[2] = VS(cur->hook, "Premia");
            v[0]+=v[2];
            break;
            
    case 10: v[0]+=0; //engineers have no premium
            break;
 
    case 2: VS(cur->hook, "KWagePrem"); //ensures that premia are updated
            v[2] = VS(cur->hook, "KPremia");
            v[0]+=v[2];
            break;
            
            
    case 20: 
            v[0]+=0; //engineers have no premium
            break;
            
    case 3: VS(cur->hook, "WagePremEF"); //ensures that premia are updated
            v[2] = VS(cur->hook, "PremiaEF");
            v[0]+=v[2];
            break;
  
    default:
            INTERACTS(cur, "Error Premia", v[1]);
            break;       
   }              
   
}

RESULT(v[0] )


EQUATION("ShareIncome")
/*
Share of total income for this class
*/

v[0]=V("Income");
v[1]=VS(p->up,"TotIncome");
RESULT(v[0]/v[1] )

EQUATION("ShareIndividuals")
/*
Share of total income for this class
*/

v[0]=V("Individuals");
v[1]=VS(p->up,"TotIndividuals");
RESULT(v[0]/v[1] )

EQUATION("ShareWageIncome")
/*
Share of total wage for this class
*/

v[0]=V("WageIncome");
v[1]=VS(p->up,"TotWage");
RESULT(v[0]/v[1] )


EQUATION("SharePremiaIncome")
/*
Share of total premia for this class
*/

v[0]=V("PremiaIncome");
v[1]=VS(p->up,"TotPremia");
if(v[1]==0)
 v[2]=0;
else
 v[2]=v[0]/v[1];
RESULT(v[2] )

EQUATION("DecilesRatios")
/*
Compute a few statistics:
- Income of top 1%
- Income of top 10%
- Ratio of 1%/90%
- Ratio of 10%/90%
*/

v[0]=v[1]=v[2]=v[3]=v[21]=v[22]=v[23]=0;
SORT("Class","IncomeCapita", "DOWN");
CYCLE(cur, "Class")
 {
  v[4]=VS(cur,"ShareIndividuals");
  v[7]=VS(cur,"IncomeCapita");

  v[8]=0.01;
  if(v[0]<v[8])
   {//for the 1%
    v[5]=min(v[8], v[0]+v[4]);
    v[6]=v[5]-v[0];//it is v[4], if you did not reach the threshold, or the part of it necessary to reach the top
    v[1]+=v[7]*v[6];
    v[21]+=v[6];
   }
  v[8]=0.1;
  if(v[0]<v[8])
   {//for the 10%
    v[5]=min(v[8], v[0]+v[4]);
    v[6]=v[5]-v[0];//it is v[4], if you did not reach the threshold, or the part of it necessary to reach the top
    v[2]+=v[7]*v[6];
    v[22]+=v[6];
   }
  v[0]+=v[4];
 }
WRITE("IncomeTop1",v[41]=v[1]*100);
WRITE("IncomeTop10",v[42]=v[2]*10);

v[0]=0;
SORT("Class","IncomeCapita", "UP");
CYCLE(cur, "Class")
 {
  v[4]=VS(cur,"ShareIndividuals");
  v[7]=VS(cur,"IncomeCapita");

  v[8]=0.90;
  if(v[0]<v[8])
   {//for the 90%
    v[5]=min(v[8], v[0]+v[4]);
    v[6]=v[5]-v[0];//it is v[4], if you did not reach the threshold, or the part of it necessary to reach the top
    v[3]+=v[7]*v[6];
    v[23]+=v[6];
   }
  v[0]+=v[4];
 }
WRITE("IncomeBottom90",v[43]=v[3]/(0.9));

WRITE("Ratio190",v[41]/v[43]);
WRITE("Ratio1090",v[42]/v[43]);
SORT("Class","NumClass", "UP");
RESULT(1 )

EQUATION("GlobalIncome")
/*
Differentiate from TotIncome because considers also the financial rents
*/

v[0]=0;
CYCLE(cur, "Class")
 {
  v[0]+=VS(cur,"Income");
 }

RESULT(v[0] )


EQUATION("UnitLaborCost")
/*
Unit labor cost
*/

v[21]=v[22]=v[4]=v[26]=0;
v[30] = V("PayOvertime");
CYCLE(cur, "Labor")
 {
  //v[16]=VS(cur,"wage")/v[30];
  v[16]=VS(cur,"wage");
  v[17]=VS(cur,"NumWorkers");

  if(v[17]==0)
  	END_EQUATION(0);

  v[25]=VLS(cur,"NumWorkers",1);
  if(v[4]==0)
   {
    v[5]=v[17];
    v[4]=1;
   }
  v[21]+=v[16]*v[17];
  v[22]+=v[17]; // total labour force
  v[26]+=v[25]; //total lagged labour force
 }

if(v[22]>0)
 v[23]=v[21]/v[22];
else
 v[23]=0;
WRITE("LaborForce",v[22]);
WRITE("LaborForceL",v[26]);

if(v[26]>0)
	v[30]=v[22]/v[26]-1;
else
	v[30]=0;

WRITE("LaborGrowth",v[30]);
/*
v[0]=V("CapitalCapacity");
v[1]=V("LaborCapacity");
v[2]=min(v[0],v[1]);
v[24]=v[21]/v[2];
*/

v[2]=V("MaxLaborProductivity");
if(v[5]>0 && v[2])
 v[24]=v[21]/(v[2]*v[5]);
else
 v[24]=v[21];
RESULT(v[24] )




EQUATION("MovAvProfit")
/*
Moving average of the profits.
Test to be uases as an indicator of profits that is not oscillating as unit sales, and allows to use profits to trigger exit from market
*/

v[1]=VL("MovAvProfit",1);
v[2]=VL("Profit",1);
v[3]=V("aProfit");
v[4]=v[3]*v[2]+(1-v[3])*v[1];

RESULT(v[4] )


EQUATION("ExpectedProfit")
/*
Level of profits perceived by the firm, which reflects the expected gains from the sales.
To be used as an indicator of profit that is not oscillating as unit sales, and that would allow to use profits to trigger exit from markets
*/
v[0]=V("ExpectedSales");
//v[1]=V("price");
v[1]=VL("price",1);
v[2]=V("LaborCost");
v[4]=V("RdExpenditure");
v[5]=V("ENCost");
v[6] = V("CapitalCostF");

v[3]=v[0]*v[1]-v[2]-v[4] - v[5]-v[6];

RESULT(v[3] )



EQUATION("LaborCost")
/*
Comment
*/
v[0]=0;
CYCLE(cur, "Labor")
 {
  v[1]=VS(cur,"NumWorkers");
  v[2]=VS(cur,"wage");
  v[0]+=v[1]*v[2];
 }

RESULT(v[0] )



EQUATION("ExpectedProfit2")
/*
Level of profits perceived by the firm, which reflects the gains from the desired production
*/
v[0]=V("DesiredQ");
//v[1]=V("price");
v[1]=VL("price",1);
v[2]=V("LaborCost");
v[5]=V("ENCost");
v[6] = V("CapitalCostF");

v[3]=v[0]*v[1]-v[2]-v[5] -v[6];
RESULT(v[3] )


EQUATION("ExpectedProfit3")
/*
Level of profits perceived by the firm, which reflects the gains from the actual production
*/
v[0]=V("Q");
//v[1]=V("price");
v[1]=VL("price",1);
v[2]=V("LaborCost");
v[5]=V("ENCost");
v[6] = V("CapitalCostF");

v[3]=v[0]*v[1]-v[2] -v[5] - v[6];
RESULT(v[3] )



EQUATION("IncLearningK")
/*
Express the skills of the firm in exploiting the capital, where 0 is the inability to use the capital and
1 is the maximum exploitation
*/

v[0]=V("aLearningK");
v[1]=CURRENT*v[0]+(1-v[0]);
RESULT(v[1] )



EQUATION("MaxKQ")
/*
Defines the Theoretical Labor Productivity of the Firm as incorporated in the various capital vintages of the firm.
*/
v[3]=V("FDKCapacity");
v[9]=V("CapitalIntens");
v[19]=V("IncLearningK");
v[7]=v[3]*v[19];//computes the actual stock of this capital vintage that can be used
v[8]=v[7]/v[9];
RESULT(v[8] )


EQUATION("MaxLaborProductivity")
/*
Defines the Theoretical Labor Productivity of the Firm as incorporated in the various capital vintages of the firm.
*/

v[10]=V("CapitalStock");
if(v[10]==0)
	END_EQUATION(VL("AvMaxLaborProductivity", 1));

v[0]=v[1]=v[41]=v[42]=0;

v[20]=0;
v[30]=VL("Inflation",1);
CYCLE_SAFE(cur, "Capital")
 {
  v[7]=VS(cur,"FDKCapacity");
  v[5]=VS(cur, "IncProductivity");
  v[0]+=(v[7]*v[5]);
  v[1]+=v[7];
  v[25]=VS(cur, "IncEfficiency");
	v[20]+=(v[7]*v[25]);
 }
v[8]=v[0]/v[1];//Max Labor productivity computed as the weighted average of the incorporated productivity in every capital vintages

v[28]=v[20]/v[1];
WRITE("MaxEfficiency", v[28]);

RESULT(v[8] )

EQUATION("AvMaxLProd")
/*
Average Max theoretical labour productivity across firms, to be used to index min wages, as this productuvity is what can be shared between capital and labour
*/

v[3]=0;
CYCLE(cur1, "Supply")
 {
  CYCLES(cur1, cur, "Firm")
   {
    v[1]=VS(cur,"MaxLaborProductivity");
    v[2]=VS(cur,"Ms");
    v[3]+=v[1]*v[2];
   }
 }

RESULT(v[3] )

EQUATION("MovAvMaxLProd")
/*
Exponential Moving Average of the aggregate productivity
*/

v[1]=VL("MovAvMaxLProd",1);
//v[2]=VL("AggProductivity",1);
v[2]=VL("AvMaxLProd",0);
v[3]=V("aAgProd");
v[4]=v[3]*v[2]+(1-v[3])*v[1];

if(t==1)
 v[4]=v[2];
RESULT(v[4] )

EQUATION("KAge")
v[0]=VL("KAge",1);
RESULT((v[0]+1) )

EQUATION("PayOvertime")
/*
Extra wage paid to workers producing more than their standard capacity.
The minimum is 1 (no overtime).
The maximum is the ratio between actual production and standard capacity
*/
v[0] = V("LaborCapacity");
v[1] = V("Q");
if(v[1]<=v[0] || v[0]==0)
 END_EQUATION(1);

v[2]=1+log(v[1]/v[0]);
RESULT(v[2] )


EQUATION("wage")
/*
Wage of the workers in this layer of the firm
*/
v[5]=V("NumWorkers");
v[2]=V("IdLabor");
v[25] = V("PayOvertime");
if(v[2]==1)
 { // first tier workers
  v[0]=V("MinWage")*v[25];
  v[1]=V("wagecoeff"); // wage coefficient as the minimum wage multiplier
 }
else
 { // executives
  cur=SEARCH_CNDS(p->up,"IdLabor",v[2]-1);
  v[0]=VS(cur,"wage");
  v[1]=V("wagecoeff"); // wage coefficient as the wage tier multiplier
 }

/*
VS(p->hook,"PayTime");
if(V("flagShowWage")==1)
 INTERACT("Income push", v[5]*v[0]*v[1]);
INCRS(p->hook,"Individuals",v[5]);
INCRS(p->hook,"WageIncome",v[5]*v[0]*v[1]);
*/

RESULT((v[0]*v[1]) )






EQUATION("KLaborCost")
/*
Comment
*/
v[0]=v[1]=v[2]=0;
CYCLE(cur, "KLabor")
 {
  v[0]+=VS(cur,"KNbrWorkers")*VS(cur,"KWage");
  v[1]+=VS(cur,"KNbrWorkers");
  v[2]+=VLS(cur,"KNbrWorkers",1);
 }
v[3]=V("KNbrEngineers");
v[4]=VL("KNbrEngineers",1);
WRITE("KLaborForce",v[1]+v[3]);
WRITE("KLaborForceL",v[2]+v[4]);
WRITE("KLaborGrowth",(v[1]+v[3])/(v[2]+v[4])-1);
v[0]+=V("KNbrEngineers")*V("KWageEngineers");
RESULT(v[0] )





/****************************************************************/
/******************** PROUCT INNOVATION *************************/
/****************************************************************/


EQUATION("RdExpenditure")
/*
Cumulated profits (not used to invest in kapital) devoted to product R&D
*/

V("InvestmentDecision");
v[0]=V("MovAvExpSales");

v[2]=V("roRD");
v[3]=v[2]*v[0];
//LOG("%lf \t %lf \n RdExp",v[3],1);
if(v[3]<0)
	END_EQUATION(0 ); // Avoid 'RdExpenditure' produces the invalid value 'nan'

v[1]=log(1+v[3]);

RESULT(v[1] )



EQUATION("ProdInno")
/*
The actual product innovation: extraction of a quality given the quality of the good the firm is currently producing
*/


if(INCR("tInno",1)<0)
 END_EQUATION(0);

v[17]=V("RdExpenditure");

V("Trade");
cur1=SEARCH_CND("IdPNeed",0);
cur=SEARCH_CNDS(cur1,"IdCh",2);
v[2]=VS(cur,"x"); // check the current quality level of the produced good
v[4]=V("product"); // the sector in which the firm is currently producing
v[6]=V("ProdShockP")*v[2]; // productivity shock that determines the variance of the product innovation


v[0]=V("zInno"); //probability of hitting an innovation
for(v[14]=v[9]=0; v[9]<v[17]; v[9]++)
{
 if(RND<v[0])
 {
   v[8]=norm(v[2],v[6]); // outcome of the product innovation
   if(v[8]>v[2])
    {
     v[14]++;
     WRITELS(cur,"x",v[8],t);
     v[2]=v[8];
     v[10]=V("innoInterval");
     WRITE("tInno",-1*v[10]);
    }
 }
}

RESULT(v[14] )

EQUATION("MovAvExpSales")
/*
Moving average of expected sales.
Used by the firm to decide wether to adopt a prototipe
*/

v[1]=VL("MovAvExpSales",1);
v[2]=VL("ExpectedSales",1);
v[3]=V("aExpSales");
v[4]=v[3]*v[2]+(1-v[3])*v[1];
RESULT(v[4])



EQUATION("msEntrants")
/*
Estimation of the capital newly entered firm need to order
*/
v[1]=V("NbrFirms");
v[0]=0.1/v[1];
RESULT(v[0] )



EQUATION("newFirmKNeed")
/*
Estimation of the capital newly entered firm need to order
*/
v[0]=v[1]=v[2]=0;
CYCLE(cur, "sFirm")
 {
  v[0] += VS(cur->hook, "CapitalStock");
 }
v[1] = V("msEntrants");
v[2]=v[0]*v[1];
if(v[2]==0)//***
	v[2]=100;//***
RESULT(v[2] )

EQUATION("KapitalNeed")
/*
Decide whether to order new capital.
*/


v[0]=V("Waiting");
if(v[0]==1)
 END_EQUATION(CURRENT);

V("MaxLaborProductivity");
v[3]=V("CapitalCapacity");
v[42] = V("CapitalLife");
v[45] = V("AvWaitTime");
v[43]=0;
CYCLE(cur, "Capital")
{//compute the K capacity five periods before scrapping
	v[44] = VS(cur, "KAge");
	if(v[44]==v[42]-5)
	 v[43] += VS(cur, "FDKCapacity");
}

v[4]=V("ExpectedSales");
//v[5]=V("backlog")/10;//a tenth of backlog should be got rid of.
v[5]=V("backlog");//a tenth of backlog should be got rid of.

v[7]=V("DesiredUnusedCapacity");
v[8]=V("CapitalIntens");
v[50]=VL("Stocks",1);
v[51]=V("DesiredStocks"); // percentage of expected sales firms want to stock to face unexpected demand +1


//v[9]=(v[4]+v[5])*v[7];
v[9]=(v[4]*(1+v[51])-v[50]*0.1+v[5])*v[7]; // add stocks' gap to the equation


v[6] = V("ExtraCapacity");
v[23]=V("LaborCapacity")*v[6];
v[24]=min(v[9],v[23]);//increase K if it is the bottleneck, considering also LaborCapacity
v[10]=v[24]-v[3]+v[43];
v[11]=max(v[10],0);
if(v[11]==0)
 END_EQUATION(0);

v[12]=v[11]*v[8];//desired capital

//if(v[11]+v[3]-v[43]>V("LaborCapacity")*1.5)
  //INTERACT("Huge KapitalNeed",v[11]);
END_EQUATION( v[12]);

/////// CANCELED ///////////////
v[14]=VL("SmoothProfit",1);
v[16]=V("AvKPrice");
v[17]=V("InterestRate");
v[24]=V("BacklogValue");

v[18]=max(0,v[14]) ;//financial constraints


v[20]=VL("NetWorth",1)-V("DebtF");//Financial feasibility xxx
if(v[20]>=v[12]*v[16])
 {
  WRITE("RationingRatioFirm",1);
  END_EQUATION(v[12]);
 }

v[21]=v[20]/(v[16]*v[12]);
v[12]*=v[21];

WRITE("RationingRatioFirm",v[21]);


RESULT(v[12] )




EQUATION("SmoothProfit")
/*
Comment
*/
v[1]=CURRENT;
if(v[1]==-1)
  v[0]=V("Profit");
else
  v[0]=v[1]*0.9+0.1*V("Profit");
RESULT(v[0] )



EQUATION("NetWorthXXX")
/*
Measure of the value of the firm in case of sales
*/

v[0]=V("KPresentValue")*V("PrudenceLending");
v[1]=V("CashF");
v[2]=V("OutstandingDebtF");//xxx
v[4]=V("BacklogValue");

v[3]=v[0]+v[4]-v[2]+v[1];
RESULT(v[3] )








EQUATION("KPresentValue")
/*
Present value of capital
*/

v[0]=v[1]=0;
v[5]=V("AvCurrProd");
v[2]=V("AvKPrice");
CYCLES(p->up, cur, "Capital")
 {
  v[3]=VS(cur,"IncProductivity");
  v[4]=VS(cur,"FDKCapacity");

  v[0]+=v[2]*v[4]*v[3]/v[5];
 }

RESULT(v[0] )

EQUATION("AvCurrProd")
/*
Av. current productivity
*/

v[0]=v[1]=0;
CYCLE(cur, "KFirm")
{
	v[0] = VLS(cur, "CurrentProductivity",1);
	v[1]++;
}
v[2]=v[0]/v[1];
RESULT(v[2] )


EQUATION("PlaceOrder")
/*
Place the order from the calling firm to a Kapital producer adopting the technology of the firm
 */
 v[0]=t;
 v[1] = V("stopPlaceOrder");
//sprintf(msg, "\n PlaceOrder %lf", v[0] ); plog(msg);
if(v[0]>v[1])
 END_EQUATION(0);

  v[0]=VS(c,"IdTech"); //this is the technology of the final product firm
  v[30]=VS(c,"betaPrice");
  v[31]=VS(c,"betaProd");
  v[32]=VS(c,"betaTime");
  v[11]=VS(c,"betaEff"); //add betaEff as a parameter
  v[3]=VS(c,"KapitalNeed");
  v[74] = VS(c, "MaxKExpenditureF");

v[99] = VS(c, "flagNewEntrant");
//if(v[99]==1)
 //INTERACT("ne", v[99]);

//assuming there are many firms producing K with the same technologies, firm select the one they prefer in terms of price and productivity of the capital, and waiting time (insert also durability of the capital if we include depreciation as a function of production quantity and not time)
v[51]=v[53]=v[59]=v[60]=v[58]=v[70]=v[11]=v[12]=v[13]=v[14]=v[15]=v[16]=v[17]=v[18]=0;
//given the preference of the buyer firm wth respect to the features of the capital production, namely price and current productivity of the capital, and approximate time to wait for receiving the order & the energy efficiency


CYCLE(cur, "KFirm")
{
  v[14]=VS(cur,"KType");
	if(v[14]==2) // compute average only for KFirms producing final goods
	{

  v[50]=VLS(cur,"KPrice",1);
  v[51]+=v[50];
  cur2=SEARCHS(cur,"KCapital");
  v[52]=VLS(cur2,"CurrentProductivity",1);
  v[12]=VLS(cur2,"CurrentEfficiency",1); // Efficiency of the vintage
  v[53]+=v[52];
  v[13]+=v[12];
  v[60]++;
  v[64]=VS(cur,"NumOrders");

  v[54]=VS(cur,"KQ");//number of productive workers
  // WRITES(cur,"WaitTime",ceil(v[3]/v[54]));
  WRITES(cur,"WaitTime",1);
  if(v[64]>0)
  {
    CYCLES(cur, cur1, "Order")
    {

      v[55]=VS(cur1,"KCompletion");
      v[56]=VS(cur1,"KAmount");
      v[57]=(v[56]-v[55])/v[54];
      INCRS(cur,"WaitTime",v[57]);
    }
  }

  v[65]=VS(cur,"WaitTime");
  if(v[65]<0)
    INTERACTS(c, "Neg. waittime", v[65]);
  v[58]+=v[65];
  // an index that gives the amount of time needed or a firm to complete the production of the capital already ordered
  }
}
// write the average values for price  and producitivty of capital, and for the index of awaiting time. The averates are updated each time a firm place an order, as this would affectthe choice of a following firm in the same time period.
// NOTE THAT THIS PROVIDES A FIRST MOVER 'ADVANTAGE' TO THE FIRST FIRMS.
v[61]=v[51]/v[60];
WRITE("AvKPrice",v[61]);
v[62]=v[53]/v[60];
//WRITE("AvCurrProd",v[62]);
v[14]=v[13]/v[60];
WRITE("AvCurrEff",v[14]); // Average Current Efficiency
v[63]=v[58]/v[60];
WRITE("AvWaitTime",v[63]);


//v[17]=V("MinWage");
//v[18]=V("PriceEN");
v[81]=-9999;

cur1=SEARCH("KFirm");
CYCLE(cur, "KFirm")
{
  v[14]=VS(cur,"KType");
	if(v[14]==2) // compute average only for KFirms producing final goods
	{
  v[1]=VS(cur,"IdKTech");
  if(v[0]!=v[1])
    WRITES(cur,"kapp",0);
  else
    WRITES(cur,"kapp",1);
  v[20]=VLS(cur,"KPrice",1);
  v[21]=VLS(cur,"CurrentProductivity",1);
  v[15]=VLS(cur,"CurrentEfficiency",1); // retrieve current efficiency
  v[26]=VS(cur,"WaitTime");
  // normalise relative values (shares) in order to compare firms on the different indicators using the same evaluation for different units of measurement
  v[27]=v[20]/v[61]+1; // Normalize Price
  v[28]=v[21]/v[62]+1; // Normalize Productivity
  v[16]=v[15]/v[14]+1; // Normalize Efficiency
  v[29]=v[26]/v[63]+1; // Normalize Wait Time
  if(v[27]*v[28]*v[29]*v[16]==0)
    v[33]=0;
  else// Do not include energy price and minimum wage here
	  {
	  //v[33]=RND;
	  v[33]=v[28]*v[31] - v[29]*v[32] - v[27]*v[30] + v[16]*v[11]; //sprintf(msg, "\n v33 %lf", v[33] ); plog(msg);
	  //v[33]=pow(v[28],v[31]) - pow(v[29],v[32]) - pow(v[27],v[30]) + pow(v[16],v[11]);
    //v[33]=pow(v[28],v[31])*pow(v[29],-v[32])*pow(v[27],-v[30])*pow(v[16],v[11]);
	  }


  //v[33]=pow(v[28]*v[17],v[31])*pow(v[29],-v[32])*pow(v[27],-v[30])*pow(v[16]*v[18],v[11]); // Includes PriceEN and MinWage
  WRITES(cur,"kselect",v[33]*VS(cur,"kapp"));
  v[70]+=v[33];
  if(v[33]>=v[81])
  {
    v[81]=v[33];
    cur1=cur;
  }

	}
}

      //Legend:
      //c: it is the final producer firm ordering the K
      //cur: is the K producer

      cur=cur1;
      v[6]=VLS(cur,"KPrice",1);
      if(VS(cur,"NumOrders")==0)
        cur1=SEARCHS(cur,"Order");
      else
        cur1=ADDOBJS(cur,"Order");

//      if(V("Age")<=2) // Allow new firms to place order
//      	v[63]=min(v[3],v[74]/v[6]); // v[3]=V("KapitalNeed")
//       else
     	v[63]=v[3];
      WRITES(cur1,"KAmount",v[63]);
      if(v[63]<=0)
        INTERACTS(c,"Neg.KAmount",v[63]);
      WRITES(cur1,"KCompletion",0);
      WRITES(cur1,"TimeWaited",1);
      WRITES(cur1, "EnergyKOrder", 0);
      cur1->hook=c; //useful to retrieve quickly the ordering firm


      INCRS(cur,"NumOrders",1);
      v[4]=VLS(cur,"CurrentProductivity",1); //current state of the K art
      WRITES(cur1,"Kproductivity",v[4]); //tech characteristics of the capital stock order

      v[90]=VLS(cur,"CurrentEfficiency",1); //lagged CurrentEfficiency, correct to non lagged if necessary
      WRITES(cur1,"KEfficiency",v[90]); // efficiency of the capital stock order

      //v[5]=VLS(cur,"CurrentSkillBiais",1);
      //WRITES(cur1,"KSkillBiais",v[5]);
      WRITES(cur1,"KP",v[6]);// write the price of the capital in the period in which it is ordered, and use it to compute the actual expenditure using the `agreed' price.

        v[72]=VS(c,"IdFirm");

      WRITES(cur1,"IdClient",v[72]);

RESULT(1 )



EQUATION("KNbrEngineers")
/*
Number of enginers is a share of the number of blue collars.
*/

v[8]=V("EngineersShare");
v[9]=VS(p->up,"KNbrWorkers"); // number of first tier worker as a max to chose the number of engineers
v[11]=v[9]*v[8];
v[55]=VS(p->up,"KRatioVacancies");
INCRS(p->up,"KVacancies",v[55]*v[11]);

RESULT(v[11] )


EQUATION("KWageEngineers")
/*
Wage of engineers
*/
v[0]=V("MinWage");
v[1]=V("KEWagecoeff");
//VS(p->hook->up,"PayTime");
v[3]=V("KNbrEngineers");
/*
VS(p->hook->up,"PayTime");
INCRS(p->hook,"WageIncome",v[1]*v[0]*v[3]);
INCRS(p->hook,"Individuals",v[3]);
*/
RESULT((v[0]*v[1]) )





EQUATION("KWage")
/*
Comment
*/
v[12]=V("KNbrWorkers");
v[2]=V("IdKLabor");
if(v[2]==1)
 {
  v[0]=V("MinWage");
  v[1]=V("KWagecoeff");
 }
else
 {
  cur=SEARCH_CNDS(p->up,"IdKLabor",v[2]-1);
  v[0]=VS(cur,"KWage");
  v[1]=V("KWagecoeff"); // wage coefficient as the wage tier multiplier
 }
/* 
VS(p->hook,"PayTime");
INCRS(p->hook,"Individuals",v[12]);
INCRS(p->hook,"WageIncome",v[12]*v[0]*v[1]);
*/

RESULT((v[0]*v[1]) )



EQUATION("KNbrWorkers")
/*
Number of workers in the capital sector firms
*/
v[1]=0;
CYCLES(p->up, cur, "Order")
 {
  v[11]=VS(cur,"KAmount");
  v[12]=VS(cur,"KCompletion");
  v[1]+=v[11]-v[12];
 }

v[15]=0;
CYCLES(p->up, cur, "KLabor")
 { //check how many tiers already exist  
    v[15]++;
 }   
/*
v[15]=0;
CYCLE_SAFES(p->up, cur, "KLabor")
 { //check how many tiers already exist
  if(VLS(cur,"KNbrWorkers",1)>0)
    v[15]++;
  else
   {
   INTERACTS(cur, "Remove KLabor",v[15]);
    if(SEARCHS(cur->hook->up, "LabClass")==cur->hook)
     {cur->hook->hook=NULL;
      WRITES(cur->hook, "LCType", 0);
     }
    else  
      DELETE(cur->hook);
    DELETE(cur);
   }
 }
*/ 
v[14]=V("IdKLabor");
if(v[14]==1)
 {
  v[0]=VL("KNbrWorkers",1);
  v[2]=VL("KLaborProductivity",1);
  v[4]=V("KDesiredUnusedCapacity");
  v[3]=v[4]*(v[1]/v[2]);
  v[5]=V("KaNW");
  if(v[0]>v[3])
   v[5]=0.9;
  v[6]=v[0]*v[5]+(1-v[5])*v[3];
	v[26] = VL("UnemploymentRate", 1);
  v[28]=v[6]-v[0];
  /*
  if(v[28]>0)
   {
    v[28]*=v[26];
    v[6]=v[0]+v[28];
   }
  */
  v[33]=v[3]>v[6]?v[3]-v[6]:0;
  if(v[6]>0)
   v[54]=v[33]/v[6];
  else
   v[54]=0; 
  WRITES(p->up,"KVacancies",v[33]);
  WRITES(p->up,"KRatioVacancies",v[54]);
  if(v[6]<1)
   v[6]=1; //limit the minimum number of workers to 1 to avoid crazy errors.
 }
else
 {// when above the first tier workers...
  v[18]=V("IdKLabor");
  cur=SEARCH_CNDS(p->up,"IdKLabor",v[18]-1);
  v[19]=VS(cur,"KNbrWorkers"); //and the number of workers in the previous tier
  v[21]=VS(cur,"knu"); //given the worker ratio between tiers n the tier below
  v[6]=v[19]/v[21]; // compute the required executives
  v[54]=VS(p->up,"KRatioVacancies");
  INCRS(p->up,"KVacancies",v[6]*v[54]);
  v[17]=V("knu"); //given the worker ratio between tiers in the present tier
  if(v[6]>=v[17] && v[18]==v[15])
   {
    cur1=ADDOBJ_EXS(p->up,"KLabor",p);
    WRITES(cur1,"IdKLabor",v[18]+1);
    v[20]=v[6]/v[17];
    WRITELS(cur1,"KNbrWorkers",v[20], t);
    WRITELLS(cur1,"KNbrWorkers",0, t,1); // write also that the number of workers in the previous period is 0, to be used in the statistics
    v[22]=0;
    CYCLES(p->up->up->up,cur2, "Class")
     {if(v[22]==0)
       {
        cur7=SEARCHS(cur2,"BankC");
        cur7=cur7->hook;
       }
      v[22]++;
     }
    if(v[22]-1<v[18]+1)
     { /// if there is still not a class for the new tyoe of wage earner create one
      cur2=SEARCH_CNDS(p->up->up->up,"NumClass",v[18]);
      cur3=ADDOBJ_EXS(cur2->up,"Class", cur2);
      v[31]=0;
      CYCLE_SAFES(cur3, cur4, "LabClass")
      {
       if(v[31]==0)
        {
         v[31]=1;
         WRITES(cur4, "LCType", 0);
        }
       else
        DELETE(cur4); 
      }
      cur6=SEARCHS(cur3,"BankC");
      WRITELS(cur6,"BalanceC",0, t-1);
      WRITELS(cur6,"OverdraftPaymentC",0, t-1);
      cur6->hook=cur7;
      
      cur5 = SEARCHS(cur3, "LabClass");
      cur5->hook=cur1;
      cur1->hook=cur5;
      WRITES(cur5, "LCType", 2);

      v[44]=VS(cur2,"SavingRate");
      v[45]=VS(cur2->up,"SRMultiplier");
      v[47] = VS(cur2->up, "TopSavingRate");
      v[46]=v[44]*v[45]+(1-v[45])*v[47];
      WRITES(cur3,"SavingRate",v[46]);

      WRITES(cur3,"NumClass",v[18]+1);
      WRITELLS(cur3,"Expenditure",0, t-1,1);
      WRITELS(cur3,"Expenditure",0, t);
      WRITELS(cur3,"Income",0, t-1);
      WRITELS(cur3,"WageIncome",0, t-1);
      WRITELS(cur3,"PremiaIncome",0, t-1);
      WRITELS(cur3,"Consumption",0, t-1);
      WRITELS(cur3,"GovDemHH",0, t-1);
      WRITELS(cur3,"ShareWageIncome",0, t-1);
      WRITELS(cur3,"SharePremiaIncome",0, t-1);
      WRITELS(cur3,"BankProfitIncome",0, t-1);
      WRITELS(cur3, "CashC", 0, t-1);
      WRITELS(cur3,"ShareIncome",0, t-1); // reset the share income to be recomputed
      WRITELS(cur3,"Individuals",0,t-1); // set the number of individuals to nu;ber of workers of the new class
      v[35]=VS(cur2,"LorenzInd");
      WRITES(cur3,"LorenzInd",v[35]+v[20]); // set total number of workers as previous total plus new workers
      //WRITELS(cur3,"NoConsumption",0, t-1); // set the available income due to unavailability of goods to 0
      CYCLES(cur3, cur, "Need")
       { // enter in neds and characterisitcs to change the tau parameter (the minimum is set every period)
        v[27]=VS(cur,"IdNeed");
        CYCLES(cur, cur1, "DCh")
         {
          v[26]=VS(cur1,"IdDCh");
          CYCLES(cur2, cur4, "Need")
           { // cycle among the needs of the preceding class
            v[28]=VS(cur4,"IdNeed");
            if(v[28]==v[27])
             { // when in the same need as the one the new class is cycling cycle through the char of the preceding class
              CYCLES(cur4, cur5, "DCh")
               {
                v[29]=VS(cur5,"IdDCh");
                if(v[29]==v[26])// when in the same characteristic the new class is cycling
                 v[30]=VS(cur5,"tau"); // read the value of the tau parameter
               }

             }
           }
          v[31]=VS(cur1,"tauMultiplier"); // the speed of adjustment of the tolerance level from one class to the following one
          v[34]=VS(cur1,"NegativeQuality");
          if(v[34]>0)
           v[33]=VS(p->up->up->up,"tauMax"); // the asympthotic level of the tolerance level (1 for qualities and 0 for price)
          if(v[34]<0)
           v[33]=VS(p->up->up->up,"tauMin"); // the asympthotic level of the tolerance level (1 for qualities and 0 for price)
          v[32]=v[30]*(1-v[31])+v[31]*v[33]; // adjustment in the treshold level of tolerance
          WRITES(cur1,"tau",v[32]); // finally write the tau for the new consumer class in each of its characteristic for each need
         }

       }
     v[35]=VS(cur3,"ComputeShare"); // set the expenditure shares for the needs in the new class
     }
    else
     {
      cur3=SEARCH_CND("NumClass",v[18]+1);
      if(VS(cur3, "LCType")==0)
        cur5 = SEARCHS(cur3, "LabClass");
      else  
        cur5 = ADDOBJS(cur3, "LabClass");
      cur5->hook=cur1;
      cur1->hook=cur5;
      WRITES(cur5, "LCType", 2);
     }
   }
  if(v[18]>2 && v[19]<v[21])
   v[6]=0;
 }

RESULT(v[6] )


EQUATION("KQ")
/*
Comment
*/

v[0]=V("KNbrWorkers");
v[1]=V("KLaborProductivity");

RESULT((v[0]*v[1]) )


EQUATION("MinWage")
/*
Sets the minimum wage for all categories, as an aggregate relation. Variables influecing overall wage are: aggregate productivity, inflation, and unemployment.
Aggregate productivity?
Unemployment: to account for Beveridge curves we could use the suggishness in the hiring process, which gnerates rates of vacancies...
NOTE: probably it makes sense to use levels for all variables. That is, when the variable reaches a certain level, a wage resetting is unedergone: if inflation runs too high, wages are renegotiated, if aggregate productivity increase evidently, wage are renegotiated.

*/
V("NbrWorkers");
v[0]=VL("MinWage",1);
//END_EQUATION(v[0]);
v[10]=V("InitAggProd"); //the reference level of productivity
v[2]=V("MovAvMaxLProd");
v[43]=VL("MovAvMaxLProd",1);
v[11]=V("IncrAggProd");
v[12]=v[10]+v[10]*v[11]; //required increase in productity to change the min wage
v[13]=V("MovAvPrice");
v[44]=VL("MovAvPrice",1);
v[14]=V("InitAvPrice"); //the reference level of prices
v[15]=V("IncrAvPrice");
v[16]=v[14]+v[14]*v[15]; //required increase in prices to change the min wage
v[17]=VL("LTUnemployment",1);
v[18]=VL("STUnemployment",1);
v[19]=V("STUnemployment");
v[80] = V("wPrice");

v[70]=v[13]/v[44]-1; //daily price changes
if(v[70]<0) v[70]=0;

v[71]=v[2]/v[43]-1; //daily productiviy change
if(v[71]<0) v[71]=0;

WRITE("DPrice", v[70]);
WRITE("DProductivity", v[71]);

v[72]=1.5-v[18]*5;
if(v[72]<0.5) v[72]=0.5; //share of productivity change that unemployment allows to capture

v[73]=v[70]*v[80]+v[72]*v[71]; //wage growth composed by wPrice% of inflation plus a share of productivity growth dependant on unemployment, ranging from 150% to 50%
WRITE("DMinWage", v[73]);

if(t<3)
 v[73]=0; //Initial values can be wild, so no changes in wage for the first 2 steps
v[74]=v[0]*(1+v[73]);

v[75] = V("AvEffectiveWage");
v[76]=0.95*v[74]+0.05*v[75];
v[76]=v[74];
END_EQUATION(v[76]);

/*******************/

v[30]=V("minMinWage");
v[31]=V("maxMinWage");
v[32]=V("elasMinWage");
v[33]=V("elasMWProd");
v[34]=V("elasMWPrice");

v[36]=v[2]/v[10]-1;//Change in productivity
v[37]=v[13]/v[14]-1; // Change in prices


v[39]=0;
v[40]=0;

if(v[2]>v[12])
 {//Negotiation on productivity
  v[30]=(v[12]/v[10])*v[30];
  v[31]=(v[12]/v[10])*v[31];
  v[39]=(v[33])*v[36];
  WRITE("InitAggProd",v[2]);
 // WRITE("IncrAggProd",v[11]*v[2]/v[10]);
 }


if(v[13]>v[16])
 {//Negotiation on price
  v[30]=(v[16]/v[14])*v[30];
  v[31]=(v[16]/v[14])*v[31];
  v[40]=(v[34])*v[37];
  WRITE("InitAvPrice",v[13]);
 // WRITE("IncrAvPrice",v[15]*v[13]/v[14]);
 }

if(v[2]>v[12] & v[13]>v[16])
 { //both negotiation
  v[39]=(v[33])*v[36]*0.5;
  v[40]=(v[34])*v[37]*0.5;
 }


v[48]=V("InitMinWage");

//wage curve, expressing a percentage change of wage as a function of unemployment.
if(v[19]<0.10)
 v[41]=0.001/(v[19]+0.01);
else
 v[41]=0;

//v[41]=-0.2+0.1/(v[19]+0.2);
//v[6]=v[0]+(v[32]*v[41]+v[39]+v[40])*v[48]; //***
v[6]=v[0]*(1+v[32]*v[41]+v[39]+v[40]);

v[6]=v[0]*(1+v[32]*v[41])+v[48]*(v[39]+v[40]);




if(is_nan( v[6] )!=0 || is_inf(v[6])!=0)
 {
  v[6]=v[0];
//  INTERACT("UNO", v[0]);
 }

//  INTERACT("Due", v[6]);
if(v[2]>v[12] | v[13]>v[16])
 WRITE("InitMinWage",v[6]);


WRITE("minMinWage",v[30]);
WRITE("maxMinWage",v[31]);

v[33] = V("maxMinWageGr");
if(abs(v[0]/v[6]-1)>v[33])
  v[6]>v[0]?v[6]=v[0]*(1+v[33]):v[6]=v[0]*(1-v[33]);

RESULT(v[6] )



EQUATION("AvPayOvertime")
/*
Average payovertime
*/
v[0]=v[1]=v[6]=v[7]=0;
CYCLE(cur, "Firm")
{
	v[2] = VS(cur, "LaborCost");
	v[3] = VS(cur, "PayOvertime");
	v[4] = VS(cur, "NumWorkers");
	v[5] = VS(cur, "wage");
	v[0]+=v[2]*v[3];
	v[1]+=v[2];
	v[6]+=v[4]*v[5];
	v[7]+=v[4];

}
WRITE("AvEffectiveWage", v[6]/v[7]);
RESULT(v[0]/v[1] )

EQUATION("MinWageXXX")
/*
Sets the minimum wage for all categories, as an aggregate relation. Variables influecing overall wage are: aggregate productivity, inflation, and unemployment.
Aggregate productivity?
Unemployment: to account for Beveridge curves we could use the suggishness in the hiring process, which gnerates rates of vacancies...
NOTE: probably it makes sense to use levels for all variables. That is, when the variable reaches a certain level, a wage resetting is unedergone: if inflation runs too high, wages are renegotiated, if aggregate productivity increase evidently, wage are renegotiated.

*/
V("NbrWorkers");
v[0]=VL("MinWage",1);
//END_EQUATION(v[0]);
v[10]=V("InitAggProd"); //the reference level of productivity
v[2]=V("MovAvMaxLProd");
v[43]=VL("MovAvMaxLProd",1);
v[11]=V("IncrAggProd");
v[12]=v[10]+v[10]*v[11]; //required increase in productity to change the min wage
v[13]=V("MovAvPrice");
v[44]=VL("MovAvPrice",1);
v[14]=V("InitAvPrice"); //the reference level of prices
v[15]=V("IncrAvPrice");
v[16]=v[14]+v[14]*v[15]; //required increase in prices to change the min wage
v[17]=VL("LTUnemployment",1);
v[18]=VL("STUnemployment",1);
v[19]=V("STUnemployment");
//v[18]=VL("UnemploymentRate",1);
//v[19]=V("UnemploymentRate");

v[30]=V("minMinWage");
v[31]=V("maxMinWage");
v[32]=V("elasMinWage");
v[33]=V("elasMWProd");
v[34]=V("elasMWPrice");

if(v[18]!=0)
 v[35]=v[19]/v[18]-1;
else
 v[35]=0;
v[36]=v[2]/v[10]-1;//Change in productivity
v[37]=v[13]/v[14]-1; // Change in prices
//v[36]=v[2]/v[43]-1; //Change in productivity
//v[37]=v[13]/v[44]-1; //change in price
//v[47]=v[45]/v[46]-1; //change in theoretical productivity

v[5]=v[30]+v[31]*pow(v[18],-v[32]);
//v[6]=0.95*v[0]+0.05*v[5];

if(v[35]>0)
 v[38]=(-v[32]+0.05)*v[35];
else
 v[38]=0;
v[39]=0;
v[40]=0;


if(v[2]>v[12])
 {

  v[30]=(v[12]/v[10])*v[30];
  v[31]=(v[12]/v[10])*v[31];
  v[39]=(v[33])*v[36];
  WRITE("InitAggProd",v[2]);
 // WRITE("IncrAggProd",v[11]*v[2]/v[10]);
 }


if(v[13]>v[16])
 {

  v[30]=(v[16]/v[14])*v[30];
  v[31]=(v[16]/v[14])*v[31];
  v[40]=(v[34])*v[37];
  WRITE("InitAvPrice",v[13]);
 // WRITE("IncrAvPrice",v[15]*v[13]/v[14]);
 }

if(v[2]>v[12] & v[13]>v[16])
 {
  v[39]=(v[33])*v[36]*0.5;
  v[40]=(v[34])*v[37]*0.5;
 }

/*
if(v[47]>0)
 v[39]=(v[33])*v[47];
if(v[37]>0)
 v[40]=(v[34])*v[37];
*/

v[48]=V("InitMinWage");
v[41]=-v[32]*log(v[19]/100);
v[42]=-v[32]*log(v[18]/100);
v[6]=v[0]+exp(v[41])-exp(v[42])+v[39]*v[48]+v[40]*v[48];
//INTERACT("TEST",v[41])
//v[6]=v[0]+v[39]*v[0]+v[40]*v[0];

if(is_nan( v[6] )!=0 || is_inf(v[6])!=0)
 {
  v[6]=v[0];
//  INTERACT("UNO", v[0]);
 }

//  INTERACT("Due", v[6]);
if(v[2]>v[12] | v[13]>v[16])
 WRITE("InitMinWage",v[6]);


WRITE("minMinWage",v[30]);
WRITE("maxMinWage",v[31]);

v[33] = V("maxMinWageGr");
if(abs(v[0]/v[6]-1)>v[33])
  v[6]>v[0]?v[6]=v[0]*(1+v[33]):v[6]=v[0]*(1-v[33]);

RESULT(v[6] )


EQUATION("AggProductivity")
/*
Comment
*/
v[0]=0;
v[1]=v[30]=v[31]=v[32]=v[33]=0;
CYCLE(cur2, "Supply")
 {
  CYCLES(cur2, cur, "Firm")
   {
    v[2]=VS(cur,"Q");
    v[0]+=v[2];
    v[30]++;
    v[31]+=VS(cur,"Profit");
    v[33]+=VS(cur,"markup");
//    v[32]+=VS(cur,"CumProfit");
    CYCLES(cur, cur1, "Labor")
     {
      v[3]=VS(cur1,"NumWorkers");
      v[1]+=v[3];
     }
   }
 }

WRITE("AvProfit",v[31]/v[30]);
WRITE("TotProfit", v[31]);
WRITE("AvMkup",v[33]/v[30]);
//WRITE("AvCumProfit",v[32]/v[30]);
v[4]=0;
v[5]=0;
v[10]=v[20]=v[21]=v[22]=v[23]=0;
CYCLE(cur, "KFirm")
 {v[20]++;
 	v[7]=VS(cur,"KNbrEngineers");
 	v[10]+=v[7];
    v[8]=VS(cur,"KProductionFlow");
    v[5]+=v[8];
  v[21]+=VS(cur,"KProfit");
  v[22]+=VS(cur,"KCumProfit");
  v[23]+=VS(cur,"CurrentProductivity");
   CYCLES(cur, cur1, "KLabor")
    {
     v[6]=VS(cur1,"KNbrWorkers");
  	  v[4]+=v[6];
    }

 }

WRITE("AvCurrentProductivity",v[23]/v[20]);
WRITE("AvKProfit",v[21]/v[20]);
//WRITE("AvKCumProfit",v[22]/v[0]);
v[9]=(v[0]+v[5])/(v[1]+v[4]+v[10]);
if(v[5]<0)
 INTERACT("Neg.Kprod", v[5]);
WRITE("TotalKProduction",v[5]);
RESULT(v[9] )


EQUATION("MovAvAggProd")
/*
Exponential Moving Average of the aggregate productivity
*/

v[1]=VL("MovAvAggProd",1);
//v[2]=VL("AggProductivity",1);
v[2]=VL("AvCurrentProductivity",0);
v[3]=V("aAgProd");
v[4]=v[3]*v[2]+(1-v[3])*v[1];

RESULT(v[4] )



EQUATION("Ms")
/*
Market share of each firm, computed differently only for firms in the consumable market
*/

v[1]=V("UnitSales");
v[2]=VS(p->up->up,"TotUSales");
v[3]=VL("Ms",1);
if(v[2]>0)
 v[4]=v[1]/v[2];
else
 v[4]=v[3];

RESULT(v[4] )


EQUATION("TotUSales")
/*
Total unit sold by the consumabe sector
*/

v[2]=0;
CYCLE(cur1, "Supply")
 {
  CYCLES(cur1, cur, "Firm")
   {
    v[1]=VS(cur,"UnitSales");
    v[2]+=v[1];
   }
 }

RESULT(v[2] )


EQUATION("AvExtraPrice")
/*
Weighted average price+extra price
*/

v[3]=0;
CYCLE(cur1, "Supply")
 {
  CYCLES(cur1, cur, "Firm")
   {
    v[1]=VS(cur,"price")+VS(cur, "ExtraPrice");
    v[2]=VS(cur,"Ms");
    v[3]+=v[1]*v[2];
   }
 }

RESULT(v[3] )

EQUATION("AvPrice")
/*
Weighted average price
*/

v[3]=0;
CYCLE(cur1, "Supply")
 {
  CYCLES(cur1, cur, "Firm")
   {
//    v[1]=VS(cur,"price")+VS(cur, "ExtraPrice");
    v[1]=VS(cur,"price");
    v[2]=VS(cur,"Ms");
    v[3]+=v[1]*v[2];
   }
 }

RESULT(v[3] )


EQUATION("CPI")
/*
Consumer Price Index
*/

v[1]=(double)t;
v[2]=V("MovAvPrice");
if(v[1]==1)
 WRITE("BaseP",v[2]);
v[3]=V("BaseP");
v[4]=v[2]/v[3]*100;

RESULT(v[4] )

EQUATION("MinWageIndex")
/*
Minimum Wage index
*/

v[1]=(double)t;
v[2]=V("MinWage");
if(v[1]==1)
 WRITE("BaseMinWage",v[2]);
v[3]=V("BaseMinWage");
v[4]=v[2]/v[3]*100;


RESULT(v[4] )

EQUATION("ProdIndex")
/*
Aggregate productivity Index (average theoretical productivity given firms capital goods)
*/

v[1]=(double)t;
v[2]=V("MovAvMaxLProd");
if(v[1]==1)
 WRITE("BaseProd",v[2]);
v[3]=V("BaseProd");
v[4]=v[2]/v[3]*100;

RESULT(v[4] )





EQUATION("Age")
/*
Age of the firm
*/

RESULT(CURRENT+1 )


EQUATION("MovAvPrice")
/*
Exponential Moving Average of the inflation
*/

v[1]=VL("MovAvPrice",1);
v[2]=VL("AvExtraPrice",1);
v[3]=V("aAvPrice");
v[4]=v[3]*v[2]+(1-v[3])*v[1];


RESULT(v[4] )

EQUATION("Inflation")
/*
One period change in price in the conumables market
*/

v[1]=VL("AvPrice",1);
v[2]=V("AvPrice");
v[3]=v[2]/v[1]-1;

RESULT(v[3] )




EQUATION("TotVacancies")
/*
Number of vacancies
*/

v[1]=V("NbrWorkers");
v[0]=0;

CYCLE(cur,"Firm")
 {
  v[0]+=VS(cur,"Vacancies");
  v[1]++;
 }

CYCLE(cur,"KFirm")
 {
  v[0]+=VS(cur,"KVacancies");
 }

//WRITE("AvRatioVacancies",v[0]/(v[1]+v[0])*100);
WRITE("AvRatioVacancies",v[0]/v[1]);
RESULT(v[0] )


EQUATION("MovAvTotVac")
/*
Exponential Moving Average of Total vacancies
*/

v[1]=VL("MovAvTotVac",1);
v[2]=VL("TotVacancies",1);
v[3]=V("aTotVac");
v[4]=v[3]*v[2]+(1-v[3])*v[1];

RESULT(v[4] )





EQUATION("UnemploymentRateXXX")
/*
We derive the level of unemployment using directly the Beveridge curve, without generateing it. Two options:
Linear equation: U = Constant - beta*V (Nickell et al: beta=0.23; Wall & Zoega: beta=0.5; Teo et al: 0.3>beta<0.9)
Hyperbolic equation: U = Constant + beta/V (Börsch-Supan: c<1, beta=6)
Note that both Constant and beta should be quite different when using one or the other relation. See the Earnings_and_Consumption file
*/

v[1]=V("c");
v[2]=V("AvRatioVacancies");
v[3]=V("beta");
//v[4]=v[1]-v[3]*v[2];
v[4]=v[1]+pow(v[3],3*v[2]+0.5);

//Hyperbolic
v[4]=(v[1]+v[3]/(v[2]+0.2));

v[12] = VL("NbrWorkers",1);
v[13] = V("MaxNumWorkers");

v[14]=(v[13]-v[12])/v[13];

//LOG("%lf \t %lf \n",v[4],v[14]);
v[15]=v[14]<0?0:v[14];
RESULT(v[15] )

EQUATION("LTUnemployment")
/*
Long term unemployment rate
*/

v[0]=VL("LTUnemployment",1);
v[1]=V("UnemploymentRate");
if(t==1)
 END_EQUATION(v[1]);

v[2]=V("aLTUR");

v[3]=v[0]*v[2]+(1-v[2])*v[1];
RESULT( v[3])

EQUATION("STUnemployment")
/*
Short term unemployment rate
*/

v[0]=VL("STUnemployment",1);
v[1]=V("UnemploymentRate");
if(t==1)
 END_EQUATION(v[1]);

v[2]=V("aSTUR");

v[3]=v[0]*v[2]+(1-v[2])*v[1];
RESULT( v[3])


EQUATION("NbrWorkers")
/*

*/

v[3]=v[8]=0;
CYCLE(cur2, "Supply")
 {
  CYCLES(cur2, cur, "Firm")
   {
    CYCLES(cur, cur1, "Labor")
     {
      v[1]=VS(cur1,"NumWorkers");
      v[2]=VLS(cur1,"NumWorkers",1);
      v[3]+=v[1];
      v[8]+=v[2];
     }

   }
 }
CYCLE(cur, "KFirm")
 {
  CYCLES(cur, cur1, "KLabor")
   {
    v[4]=VS(cur1,"KNbrWorkers");
    v[5]=VLS(cur1,"KNbrWorkers",1);
    v[3]+=v[4];
    v[8]+=v[5];
   }
  cur2=SEARCHS(cur,"KEngineers");
  v[6]=VS(cur2,"KNbrEngineers");
  v[7]=VLS(cur2,"KNbrEngineers",1);
  v[3]+=v[6];
  v[8]+=v[7];
 }
CYCLE(cur, "EnergyFirm")
 {
  CYCLES(cur, cur1, "LaborEF")
   {
    v[4]=VS(cur1,"NumWorkersEF");
    v[5]=VLS(cur1,"NumWorkersEF",1);
    v[3]+=v[4];
    v[8]+=v[5];
   }
 }

RESULT(v[3] )


EQUATION("MovAvNbrWork")
/*
Exponential Moving Average of the number of worwers
*/

v[1]=VL("MovAvNbrWork",1);
v[2]=VL("NbrWorkers",1);
v[3]=V("aNbrWork");
v[4]=v[3]*v[2]+(1-v[3])*v[1];

RESULT(v[4] )


EQUATION("ComputeShare")
/*
The Share is a function that is called when a classed is formed to determine the shares of expenditure across needs, as a change with respect to the previous class.
he dynamic mimics engels laws (shiting the share of expenditures toward different needs, thus reducing the expenditure on 'basic' needs, as income increases)
This Share function simply normalise the shares computed in ExpShares
*/

v[1]=SUM("ExpShare"); //compute the sum of the newly computed expenditure shares
v[4]=v[7]=0;
CYCLE(cur, "Need")
 { // cycle through the needs
  v[2]=VS(cur,"ExpShare");
  v[3]=v[2]/v[1]; // normalise the share
  WRITES(cur,"Share",v[3]); // and fix it
  v[4]+=v[3]; // check that the sum is equal to 1
  v[5]=v[3]*151; // compute the number of iterations given the consumption shares
  v[6]=round(v[5]); // round to the closest integer
  WRITES(cur,"NumIterations",v[6]);
  v[7]+=v[6]; // check that the sum of interations is 100
 }

if(abs(v[4]-1)>0.0001)
	INTERACT("Share issue", v[4]);

RESULT(v[4]+v[7] )

EQUATION("SumShare")
/*
*/
RESULT(SUM("Share") )


EQUATION("ExpShare")
/*
Change in the expenditure shares across classes
*/

v[1]=VS(p->up->up,"eta"); // iterrelations between the changes in the shares
v[2]=VS(p->up, "NumClass"); // check the current class for which the share is computed
v[5]=V("IdNeed");
v[3]=V("endExpShare"); // asymptotic value of the expenditure share for the current need (defined in the beginning symmetric to the first class distribution of shares)
CYCLES(p->up->up, cur, "Class")
 {
  v[4]=VS(cur,"NumClass");
  if(v[4]==v[2]-1)
   { // select the class below the one for which the shares are computed
    CYCLES(cur, cur1, "Need")
     { //  cycle through the different needs
      v[6]=VS(cur1,"IdNeed");
      if(v[6]==v[5])
       v[7]=VS(cur1,"Share"); // read the value of the Share
     }

   }
 }
v[8]=v[1]*(v[7]-v[3]);
v[9]=v[7]*(1-v[8]);

RESULT(v[9] )




/*******************************************************************************
INITIALISATIONS
********************************************************************************/




EQUATION("Init")
/*
Initialisation of initial values
*/

v[0]=V("InProgress");
if(v[0]==1)
 {
  V("InitInProgress");
  INTERACT("InitInProgress",1);
  END_EQUATION(-1);
 }
WRITE("InProgress",1);
V("Init_x");

v[22]=VL("MinWage",1);
CYCLE(cur4, "Supply")
 {cur7=SEARCH("Bank");
  cur6=SEARCH("Demand");
  CYCLES(cur6, cur1, "Class")
   {
    cur5=SEARCHS(cur1,"BankC");
    cur5->hook=cur7;
   }

  CYCLES(cur4, cur1, "Firm")
   {// run a first cycle trough firms to set the number of labor in t-1
    cur = SEARCHS(cur1, "Engineers");
    cur3 = SEARCH_CND( "NumClass", 0);
    cur5 = ADDOBJS(cur3, "LabClass");
    cur->hook=cur5;
    cur5->hook=cur;
    WRITES(cur5, "LCType", 10);

    cur=SEARCHS(cur1,"BankF");
    cur->hook=cur7;
    v[15]=0;
    v[16]=VS(cur1,"product");
    cur5=SEARCH_CND("IdGood",v[16]);
    cur6=SEARCHS(cur5,"sFirm");
    if(VS(cur6, "IdsFirm")!=-1)
     cur6=ADDOBJS(cur5,"sFirm");

    WRITES(cur6,"IdsFirm",VS(cur1, "IdFirm"));
    cur1->hook=cur6;
    cur6->hook=cur1;
    v[60] = VS(cur1, "CapitalStock"); //defines also the capital capacity
    v[61] = VS(cur1, "CapitalCapacity");
    v[62] = VS(cur1, "MaxLaborProductivity"); //Labor productivity
    WRITELS(cur1, "MovAvExpSales", v[61]/v[62], t-1);
    WRITELS(cur1, "ExpectedSales", v[61]/v[62], t-1);
    WRITELLS(cur1, "Q", v[61]/v[62], t, 1);
    WRITELLS(cur1, "Q", v[61]/v[62], t-1, 2);
    

    CYCLES(cur1, cur, "Labor")
     {
      v[14]=VS(cur,"IdLabor");
      cur3=SEARCH_CND("NumClass",v[14]);
      cur5 = ADDOBJS(cur3, "LabClass");
      cur->hook=cur5;
      cur5->hook=cur;
      WRITES(cur5, "LCType", 1);

      if(v[14]==1)
       { // compute the first tier workers given their productivity and production needs
        v[2]=VS(cur1,"MaxLaborProductivity");
        v[8]=VS(cur1,"CapitalCapacity");
        v[9]=v[8];
        v[4]=VS(cur1,"DesiredUnusedCapacity");
        v[3]=v[4]*(v[9]/v[2]); // number of workers in the first layer in the first period
        if(v[3]<0)
         INTERACTS(cur, "Neg numworkers", v[3]);
        WRITELLS(cur,"NumWorkers",v[3],t,1);
        WRITELS(cur,"NumWorkers",v[3],t);
        v[21]=VS(cur,"wagecoeff");
        v[23]=v[22]*v[21];
        WRITELS(cur,"wage",v[23], t-1);
        //INCRS(cur->hook, "WageIncome", v[23]);
        

       }
      else
       {// when above the first tier workers...
        v[18]=VS(cur,"IdLabor");
        v[76]=v[18]; //Maximum class
        cur2=SEARCH_CNDS(cur->up,"IdLabor",v[18]-1);
        v[17]=VS(cur2,"nu"); //given the worker ratio between tiers of the tier below
        v[19]=VS(cur2,"NumWorkers"); //and the number of workers in the previous tier
        v[6]=v[19]/v[17]; // compute the required executives
        v[24]=VS(cur,"nu"); // given the worker ration between tiers of the present tier (use different worker ratios in case we want to change it along the hierarchical structure)
        WRITELLS(cur,"NumWorkers",v[6], t,1);
        WRITELS(cur,"NumWorkers",v[6],t);
        v[25]=VLS(cur2,"wage",1);
        v[26]=VS(cur,"wagecoeff");
        v[27]=v[25]*v[26];
        WRITELS(cur,"wage",v[27], t-1);
        //INCRS(cur->hook, "WageIncome", v[23]);
        if(v[6]>=v[24] && v[18]>=2) //IMPORTANT: this init expects to be TWO (2) layers of workers, then it creates the additional ones as requested by the initial capital
         {
          //INTERACTS(cur1, "Init of labor wrong", v[18]+1);
          cur3=ADDOBJ_EXS(cur->up,"Labor",cur);
          WRITES(cur3,"IdLabor",v[18]+1);
          v[20]=v[6]/v[24];
          WRITELLS(cur3,"NumWorkers",v[20], t,1);
          WRITELS(cur3,"NumWorkers",v[20],t);
          v[28]=v[27]*v[26];
          WRITELS(cur3,"wage",v[28], t-1);
          //INCRS(cur->hook, "WageIncome", v[28]);
         }
       }

     }
   }
 }

CYCLE(cur2, "Supply")
 {v[75]=0;//TotalDebt
  CYCLES(cur2, cur, "Firm")
   {// a second cycle to set the price in t-1
    v[10]=1+V("minMarkup");
    //WRITELS(cur,"markup",v[10], t);
    v[21]=v[22]=0;
    CYCLES(cur,cur1, "Labor")
     {
      v[16]=VLS(cur1,"wage",1);
      v[17]=VLS(cur1,"NumWorkers",1);
      v[21]+=v[16]*v[17];
      v[22]+=v[17];
     }
    v[11]=v[21]/v[22];  //av. wage in the firm
    v[0]=v[31]=v[32]=0;
    v[71] = VL("KPrice", 1);
    CYCLES(cur,cur1, "Capital")
     {
      v[72] = VS(cur1, "K");
      WRITELS(cur1, "PrincipalF", v[72]*v[71], t-1);
      WRITES(cur1, "KExpenditures", v[72]*v[71]);
      v[75]+=v[72]*v[71];
      v[3]=VS(cur1,"MaxKQ");
      v[5]=VS(cur1,"IncProductivity");
      v[0]+=v[3];
      v[32]+=v[5]*v[3];
      v[31]+=v[72]*v[71];
     }
    v[9]=v[32]/v[0]; //weighted av. incProductivity
    WRITELS(cur, "OutstandingDebtF", v[31], t-1);
    WRITELS(cur,"MaxLaborProductivity",v[9],t-1);
    v[14]=v[11]*v[10]/v[9];
    if(v[14]<0 || v[9]==0)
     v[14]=INTERACT("nEG. price", v[14]);
    WRITELS(cur,"price",v[14], t-1);
    WRITES(cur,"ConstPrice",v[14]);
    v[65]=VS(cur,"LaborForce");
    WRITES(cur,"LaborForceL",v[65]);
    v[64]=VS(cur,"product");
    cur3=SEARCH_CND("IdGood",v[64]);
    INCRS(cur3,"NFirmsS",1);

   }
 }


v[3]=v[8]=v[68]=v[72]=v[73]=0;
 CYCLE( cur, "Firm")
   {v[68] += VS(cur, "K")/VS(cur, "IncEfficiency"); //assumes a single capital unit, estimates energy
    VS(cur, "CapitalStock");
    v[72]+=min(VS(cur, "CapitalCapacity"),VS(cur, "LaborCapacity"))*VLS(cur, "price", 1);

    CYCLES(cur, cur1, "Labor")
  	{
      v[1]=VS(cur1,"NumWorkers");
      v[2]=VLS(cur1,"NumWorkers",1);
      v[3]+=v[1];
      v[8]+=v[2];
     }
   }


  v[69]=0;
  v[70] = VL("KPrice",1); //Price of capital, from the first KFirm assuming it is representative
  cur4 = SEARCH("Energy");
  CYCLES(cur4, cur, "EnergyFirm")
   v[69]++;
  CYCLES(cur4, cur1, "EnergyFirm")
   {
    cur3 = SEARCHS(cur1, "CapitalEF");
    v[83] = VS(cur3, "IdCur");
    cur5 = SEARCH_CND("IdKFirm", v[83]);
    v[84] = VS(cur5, "CurrentProductivity");
    WRITES(cur3, "KENProductivity", v[84]); //align productivity of initial EN capital to that of following capital
    if(VS(cur1, "EFType")==0)
      {
       v[83] = V("MultiplierCapacityENBrown");
       v[56]=V("KOandMParaBrown");
       v[58]=V("MaxPPKAgeBrown");
      } 
    else
      {
       v[56]=V("KOandMParaGreen");
       v[58]=V("MaxPPKAgeGreen");
       v[83] = V("MultiplierCapacityENGreen");
      }

    WRITES(cur3, "CapitalEN", v[68]/v[69]);
    WRITELS(cur3, "PrincipalEF", v[70]*v[68]/(v[69]*v[83]), t-1);
    WRITELS(cur1, "OutstandingDebtEF", v[70]*v[68]/(v[69]*v[83]), t-1);
    v[57]=v[70]*v[68]/(v[69]*v[83]);
    WRITES(cur3, "KExpendituresEN", v[57]);
    WRITES(cur3,"KOandMExpenditure",(v[57]*v[56])/v[58]); // NEW
    v[75]+=v[70]*v[68]/(v[69]*v[83]);
    WRITELS(cur3, "MAUtilizationPP", 1, t-1);
    cur5 = SEARCHS(cur4, "PP");
    if(cur5->hook!=NULL)
     cur5 = ADDOBJS(cur4, "PP");
    cur3->hook=cur5;
    cur5->hook=cur3;
    WRITELS(cur5, "PricePP", 0, T-1);
    WRITELS(cur5, "PricePPDoped", 0, T-1);
    v[71] = VS(cur1, "IdFirmEN");
    WRITES(cur5, "IdEF", v[71]);

    CYCLES(cur1, cur, "LaborEF")
     {
      v[14]=VS(cur,"IdLaborEF");
      cur3=SEARCH_CND("NumClass",v[14]);
      cur2 = ADDOBJS(cur3, "LabClass");
      cur->hook=cur2;
      cur2->hook=cur;
      WRITES(cur2, "LCType", 3);

      if(v[14]==1)
       { // compute the first tier workers given their productivity and production needs
        v[8]=VS(cur, "KENProductivity");
        v[3]=(v[68]/v[69])/v[8]; // number of workers in the first layer in the first period
        WRITELLS(cur,"NumWorkersEF",v[3],t,1);
        WRITELS(cur,"NumWorkersEF",v[3],t);
        v[21]=VS(cur,"wagecoeffEF");
        v[23]=v[22]*v[21];
        WRITELS(cur,"wageEF",v[23], t-1);
        //INCRS(cur->hook, "WageIncome", v[23]);
       }
      else
       {// when above the first tier workers...
        v[18]=VS(cur,"IdLaborEF");
        v[76]=max(v[76],v[18]); //Maximum class
        cur2=SEARCH_CNDS(cur->up,"IdLaborEF",v[18]-1);
        v[17]=VS(cur2,"nuEF"); //given the worker ratio between tiers of the tier below
        v[19]=VS(cur2,"NumWorkersEF"); //and the number of workers in the previous tier
        v[6]=v[19]/v[17]; // compute the required executives
        v[24]=VS(cur,"nuEF"); // given the worker ration between tiers of the present tier (use different worker ratios in case we want to change it along the hierarchical structure)
        WRITELLS(cur,"NumWorkersEF",v[6], t,1);
        WRITELS(cur,"NumWorkersEF",v[6],t);
        v[25]=VLS(cur2,"wageEF",1);
        v[26]=VS(cur,"wagecoeffEF");
        v[27]=v[25]*v[26];
        WRITELS(cur,"wageEF",v[27], t-1);
        //INCRS(cur->hook, "WageIncome", v[27]);
        if(v[6]>=v[24] && v[18]>=2) //IMPORTANT: this init expects to be TWO (2) layers of workers, then it creates the additional ones as requested by the initial capital
         {
          cur3=ADDOBJ_EXS(cur->up,"LaborEF",cur);
          WRITES(cur3,"IdLaborEF",v[18]+1);
          v[20]=v[6]/v[24];
          WRITELLS(cur3,"NumWorkersEF",v[20], t,1);
          WRITELS(cur3,"NumWorkersEF",v[20],t);
          v[28]=v[27]*v[26];
          WRITELS(cur3,"wageEF",v[28], t-1);
          //INCRS(cur->hook, "WageIncome", v[28]);
         }
       }

     }

   }//END cycle through EnFirms



v[77]=v[75]/(v[76]+1); //INitial debt, divided among populated classes.
v[79] = V("SRMultiplier");
v[82] = V("TopSavingRate");




CYCLE_SAFE(cur, "Class")
 {
   v[78]=VS(cur, "NumClass");

	if(v[78]>v[76])
	 DELETE(cur);
	else
	 {
	  if(v[78]>1)
     {
      cur2 = SEARCH_CND("NumClass", v[78]-1);
      if(cur2==NULL)
        INTERACTS(cur, "mmm",v[78]);
      v[80] = VS(cur2,"SavingRate");
      v[81] = v[80]*v[79]+(1-v[79])*v[82];
      WRITES(cur, "SavingRate", v[81]);
      }
	  WRITELS(cur, "CashC", v[77], t-1);
	  WRITELS(cur, "Income", v[72]/(1+v[76]), t-1);
	 }
 }

CYCLE(cur, "Class")
 {
   v[78]=VS(cur, "NumClass");

   CYCLE_SAFES(cur, cur1, "LabClass")
   {
   	if(cur1->hook==NULL)
   	  DELETE(cur1);
   }
 }  

CYCLE(cur, "KFirm")
 {
  v[40]=VLS(cur,"KPrice",1);
  WRITES(cur,"KConstPrice",v[40]);
  cur1=SEARCHS(cur,"BankK");
  cur1->hook=cur7;
  CYCLES(cur, cur1, "KLabor")
   {
    v[30]=VS(cur1,"IdKLabor");
    cur3=SEARCH_CND("NumClass",v[30]);
    cur2 = ADDOBJS(cur3, "LabClass");
    cur2->hook=cur1;
    cur1->hook=cur2;    
    WRITES(cur2, "LCType", 2);
    
    v[4]=VS(cur1,"KNbrWorkers");
    v[5]=VLS(cur1,"KNbrWorkers",1);
    v[3]+=v[4];
    v[8]+=v[5];
   }
  cur2=SEARCHS(cur,"KEngineers");
  cur3=SEARCH_CND("NumClass",0);
     cur5 = ADDOBJS(cur3, "LabClass");
    cur5->hook=cur2;
  cur2->hook=cur5;

    WRITES(cur5, "LCType", 20);
      
  v[6]=VS(cur2,"KNbrEngineers");
  v[7]=VLS(cur2,"KNbrEngineers",1);
  v[3]+=v[6];
  v[8]+=v[7];
 }
cur=SEARCH("Country");
WRITELS(cur,"MovAvNbrWork",v[3],t);
//v[10]=V("AvPrice");
v[10]=1;
WRITELS(cur,"AvPrice",v[10], t-1);
WRITELS(cur,"MovAvPrice",v[10], t-1);
WRITES(cur,"InitAvPrice",v[10]);
//WRITELS(cur,"MinWage",v[10],1);
//v[11]=V("AggProductivity");
v[11]=1;
WRITELS(cur,"MovAvAggProd",v[11], t-1);
WRITELS(cur,"AggProductivity",v[11], t-1);
WRITES(cur,"InitAggProd",v[11]);

V("MinimumInit"); // assign the value for the Minimum parameter to all classes
V("TauInit");
V("ShareInit"); // assign the expenniture shares to the classes above the first one

V("CheckLaborClass");

PARAMETER;
RESULT(1 )

EQUATION("CheckLaborClass")
/*
Control that each worker is correctly mirrored in the right consumer class
 */

CYCLE(cur, "Firm")
 {
 cur1 = SEARCHS(cur, "Engineers");
 v[0]=0;
 CYCLES(cur1->hook->up, cur2, "LabClass")
 {
  if(cur2->hook==cur1)
   v[0]++;
 }
 if(v[0]!=1)
  INTERACTS(cur1, "LabClass error (engineers)", v[0]);
  
// INCRS(cur1->hook, "appRE", 1);
 CYCLES(cur, cur1, "Labor")
   {
    v[0]=0;
    CYCLES(cur1->hook->up, cur2, "LabClass")
    {
     if(cur2->hook==cur1)
      v[0]++;
    }
    if(v[0]!=1)
     INTERACTS(cur1, "LabClass error (Labor)", v[0]);
   }   
 }

CYCLE(cur, "KFirm")
 {
  cur1 = SEARCHS(cur, "KEngineers");
  v[0]=0;
  CYCLES(cur1->hook->up, cur2, "LabClass")
  {
   if(cur2->hook==cur1)
    v[0]++;
  }
  if(v[0]!=1)
   INTERACTS(cur1, "LabClass error (Kengineers)", v[0]);
  CYCLES(cur, cur1, "KLabor")
   {
    v[0]=0;
    CYCLES(cur1->hook->up, cur2, "LabClass")
    {
     if(cur2->hook==cur1)
      v[0]++;
    }
    if(v[0]!=1)
     INTERACTS(cur1, "LabClass error (KLabor)", v[0]);
   }    
 }

CYCLE(cur, "EnergyFirm")
 {
 CYCLES(cur, cur1, "LaborEF")
   {
    v[0]=0;
    CYCLES(cur1->hook->up, cur2, "LabClass")
    {
     if(cur2->hook==cur1)
      v[0]++;
    }
    if(v[0]!=1)
     INTERACTS(cur1, "LabClass error (LaborEF)", v[0]);
   }   
 }

RESULT(1 )


EQUATION("CheckFinBalance")
/*
Comment
*/
v[0]=v[1]=v[3]=v[4]=v[5]=v[6]=v[7]=v[8]=v[9]=v[10]=v[11]=v[12]=v[13]=v[33]=v[34]=v[40]=0;
CYCLE(cur, "Firm")
 {
  v[4] += VS(cur, "MonetarySales");
	v[2] = VS(cur, "CashF");
	if(v[2]>0)
	 v[1]+=v[2];
	else
	 {v[0]+=-v[2];
	  v[3]+=-v[2];
	 }

	CYCLES(cur, cur1, "Capital")
	{
		v[2] = VS(cur1, "PrincipalF");
		v[40]+=v[2];
		v[13]+=v[2];
		v[33]+=v[2];
	}
  CYCLES(cur, cur1, "Labor")
  {
  	v[6]+=VS(cur1, "wage")*VS(cur1, "NumWorkers");
  }
  	v[6]+=VS(cur, "NbrEngineers")*VS(cur, "WageEngineers");
  	v[8] += VS(cur, "InterestPaymentF");
  	v[9] += VS(cur, "OverdraftPaymentF");
  	v[10] += VS(cur, "InterestDepositsF");
  v[11] += VS(cur, "WagePrem");
 }

CYCLE(cur, "Class")
{
 v[5] += VS(cur, "Expenditure");
	v[2] = VS(cur, "CashC");
	if(v[2]>0)
	 v[1]+=v[2];
	else
	 {
	 v[0]+=-v[2];
	  v[3]+=-v[2];
	 }
	v[7] += VS(cur, "WageIncome");
	v[9] += VS(cur, "OverdraftPaymentC");
	v[10] += VS(cur, "InterestDepositsC");
  v[12] += VS(cur, "PremiaIncome");

}

CYCLE(cur, "KFirm")
{
	v[2] = VS(cur, "CashKF");
	if(v[2]>0)
	 v[1]+=v[2];
	else
	 {
	 v[0]+=-v[2];
	  v[3]+=-v[2];
	 }

  v[6]+=VS(cur, "KNbrEngineers")*VS(cur, "KWageEngineers");
  CYCLES(cur, cur1, "KLabor")
  {
  	v[6]+=VS(cur1, "KWage")*VS(cur1, "KNbrWorkers");

  }
 	v[9] += VS(cur, "OverdraftPaymentKF");
 	v[10] += VS(cur, "InterestDepositsKF");
  v[11] += VS(cur, "KWagePrem");
}

CYCLE(cur, "EnergyFirm")
{
	v[2] = VS(cur, "CashEF");
	if(v[2]>0)
	 v[1]+=v[2];
	else
	 {
	 v[0]+=-v[2];
	  v[3]+=-v[2];
	 }
	CYCLES(cur, cur1, "CapitalEF")
 	{
  	v[2] = VS(cur1, "PrincipalEF");
		v[40]+=v[2];
		v[13]+=v[2];
		v[34]+=v[2];
 	}
	CYCLES(cur, cur1, "LaborEF")
  {
  	v[6]+=VS(cur1, "wageEF")*VS(cur1, "NumWorkersEF");
  }
 	v[8] += VS(cur, "InterestPaymentEF");
 	v[9] += VS(cur, "OverdraftPaymentEF");
 	v[10] += VS(cur, "InterestDepositsEF");
  v[11] += VS(cur, "WagePremEF");

}

v[60] = V("GovernmentFund");
v[61] = V("ExtraGovSpending");

WRITE("TotKFirm", v[33]);
WRITE("TotKEF", v[34]);
v[35] = V("BankCapital");

v[71] = V("TotalDepositsF");
v[72] = V("TotalDepositsC");
v[73] = V("TotalDepositsK");
v[74] = V("TotalDepositsEF");
v[75]=v[71]+v[72]+v[73]+v[74];
v[76] = V("RemovedDepositsF");
v[76]=0;
if(abs(v[1]-v[75]+v[76])/v[1]>0.000001)
 INTERACT("Wrong Deposits", v[1]-v[75]+v[76]);

v[81] = V("TotalAssetF");
v[82] = V("TotalAssetC");
v[83] = V("TotalAssetK");
v[84] = V("TotalAssetEF");

v[88]=LAST_CALC( "Exit" );
if(v[88]==(double)t)
 v[86] = V("NonPerformingLoansTotal");
else
 v[86]=0; 
//v[85]=v[81]+v[82]+v[83]+v[84]-v[87];
v[85]=v[81]+v[82]+v[83]+v[84];

//LOG("Assets: %lf; Comp.ass: %lf; Exit: %lf; Diff: %lf\n", v[85],v[0]+v[40], v[87], v[0]+v[40]-v[85]);

//overdraft+principal-sum(TotalAssetsX)
if(abs(v[0]+v[40]-v[85])/v[0]>0.000001)
 INTERACT("Wrong Assets CFB",v[0]+v[40]- v[85]);

v[87] = VL("DistributedProfitsBank",1);
//LOG("%lf\n", v[35]+v[1]-v[87]-v[0]-v[40]+v[60]);
//BankK + Deposits + DistributedProfitBank - Overdraft - Principal + Gov.Fund + ExtraGovSp - NonPerformingLoans
v[90]=v[35]+v[1]+v[87]-v[0]-v[40]+v[60]+v[61]-v[86];
v[91]=v[90]/v[0];
if(abs(v[35]+v[1]+v[87]-v[0]-v[40]+v[60]+v[61]-v[86] )/v[0]>0.00000001)
 {
  LOG("\n%lf\n", v[35]+v[1]+v[87]-v[0]-v[40]+v[60]+v[61]-v[86]);
  INTERACT("Error CFB",v[35]+v[1]+v[87]-v[0]-v[40]+v[60]+v[61]-v[86]);  
 } 
//LOG("%lf\t%lf\t%lf\t%lf\n", v[35],v[1],v[0],v[40]);
 
RESULT(v[35]+v[1]+v[87]-v[0]-v[40]+v[60]+v[61]-v[86] )


EQUATION("CheckFinBalanceLag")
/*
Comment
*/
v[0]=v[1]=0;
CYCLE(cur, "Firm")
 {
	v[2] = VLS(cur, "CashF",1);
	if(v[2]>0)
	 v[1]+=v[2];
	else
	 v[0]+=-v[2];
	CYCLES(cur, cur1, "Capital")
	{
		v[2] = VLS(cur1, "PrincipalF",1);
		v[0]+=v[2];
	}
 }

CYCLE(cur, "Class")
{
	v[2] = VLS(cur, "CashC",1);
	if(v[2]>0)
	 v[1]+=v[2];
	else
	 v[0]+=-v[2];

}

CYCLE(cur, "KFirm")
{
	v[2] = VLS(cur, "CashKF",1);
	if(v[2]>0)
	 v[1]+=v[2];
	else
	 v[0]+=-v[2];

}

CYCLE(cur, "EnergyFirm")
{
	v[2] = VLS(cur, "CashEF",1);
	if(v[2]>0)
	 v[1]+=v[2];
	else
	 v[0]+=-v[2];
	CYCLES(cur, cur1, "CapitalEF")
	{
	v[2] = VLS(cur1, "PrincipalEF",1);
		v[0]+=v[2];

	}


}
PLOG("\nDep[1].=%lf\nAss[1].=%lf\n", v[1],v[0]);
RESULT(v[1]-v[0] )

EQUATION("Init_x")
/*
Initialisation of the charactheristics (quality)
*/

CYCLE(cur, "Supply")
 {
  CYCLES(cur, cur1, "Firm")
   {
    CYCLES(cur1, cur2, "PNeed")
     {
      v[5]=VS(cur2,"IdPNeed");
      if(v[5]==0)
       { // set the initial value of the quality characteristic only for the product that is currently produced. The others are set to 0
        CYCLES(cur2, cur3, "Ch")
         {
          v[1]=VS(cur3,"IdCh");
          if(v[1]>1)
           { // if it is not a price characteristic set the initial value
            v[2]=V("Min_x");
            v[3]=V("Max_x");
            v[4]=uniform(v[2],v[3]);
            WRITELS(cur3,"x",v[4], t-1);
           }
         }
       }

     }

   }

 }

PARAMETER;
RESULT(1 )


EQUATION("InitMarkup")
/*
Set the initial level of mark-up
*/

v[1]=V("Avx"); // average value of the quality characteristic across firms (computed from the initial setting)
v[2]=V("AvMarkup"); // average markup, used as a parameter to change the difference in firms pricing with respect to quality setting
v[8]=v[9]=0;
CYCLE(cur, "Supply")
 {
  CYCLES(cur, cur1, "Firm")
   {
    v[3]=VS(cur1,"AvxFirm");
    v[4]=(v[3]-v[1])/v[3]; // diffrence from the average value of quality
    v[5]=v[2]*v[4];
    v[6]=v[2]+v[5]; // equal change in the markup
    WRITES(cur1,"markup",1+v[6]);
    v[7]=VS(cur1,"markup");
    if(v[7]<1)
     INTERACT("A negative Markup setting. Check equation InitMarkup", v[6]);
    v[8]+=v[7];
    v[9]++;
   }

 }
v[10]=v[8]/v[9]; // check it is the same value of AvMarkup

PARAMETER;
RESULT(v[10] )


EQUATION("AvxFirm")
/*
Average level of quality characteristic in the firm (used to set the initial mark-up), turns into a parameter
*/

v[3]=v[4]=0;
CYCLE(cur, "PNeed")
 {
  CYCLES(cur, cur1, "Ch")
   {
    v[1]=VS(cur1,"IdCh");
    if(v[1]>1)
     {
      v[2]=VS(cur1,"x");
      v[3]+=v[2];
      v[4]++;
     }
   }

 }
v[5]=v[3]/v[4];

PARAMETER;
RESULT(v[5] )


EQUATION("MinimumInit")
/*
Initialisation of the minimum parameter across classes, needs and chars
*/

v[2]=V("MinimumPMinE"); // minimum value of the minimum parameter on price for engineers
v[3]=V("MinimumPMaxE"); // maximum value of the minimum parameter on price for engineers
v[4]=V("MinimumPMinW"); // minimum value of the minimum parameter on price for workers
v[5]=V("MinimumPMaxW"); // maximum value of the minimum parameter on price for workers
v[13]=V("MinimumCMinE"); // minimum value of the minimum parameter on any quality characteristic for engineers
v[14]=V("MinimumCMaxE"); // maximum value of the minimum parameter on any quality characteristic for engineers
v[15]=V("MinimumCMinW"); // minimum value of the minimum parameter on any quality characteristic for workers
v[16]=V("MinimumCMaxW"); // maximum value of the minimum parameter on any quality characteristic for workers
v[6]=1000000;
CYCLE(cur, "Supply")
 {
  CYCLES(cur, cur1, "Firm")
   {
    v[7]=VLS(cur1,"price",1);
    if(v[7]<v[6])
     v[6]=v[7];
   }

 }
v[10]=V("AvWagecoeff");
CYCLE(cur, "Demand")
 {
  CYCLES(cur, cur1, "Class")
   {
    v[1]=VS(cur1,"NumClass");
    if(v[1]<1)
     { // engineers
      CYCLES(cur1, cur2, "Need")
       {
        CYCLES(cur2, cur3, "DCh")
         {
          v[2]=VS(cur3,"IdDCh");
          if(v[2]==1)
           { // maintain the minimum above the minimum of the characteritic (price) only for the fist (price) characteritic
            v[8]=uniform(v[2],v[3]);
            v[9]=VS(cur3,"Delta"); // variance of the distribution of the perceived characteritic around the actaul value
            v[11]=v[9]*v[6];
            v[12]=max(v[8],v[11]); // use either the initially set minimum or the price of the cheapest firm
            WRITES(cur3,"Minimum",v[12]);
           }
          else
           {
            v[17]=uniform(v[13],v[14]);
            WRITES(cur3,"Minimum",v[17]);
           }
         }

       }

     }
    if(v[1]==1)
     {// first tier workers
      CYCLES(cur1, cur2, "Need")
       {
        CYCLES(cur2, cur3, "DCh")
         {
          v[17]=VS(cur3,"IdDCh");
          if(v[17]==1)
           {
            v[18]=uniform(v[4],v[5]);
            v[19]=VS(cur3,"Delta");
            v[20]=v[19]*v[6];
            v[21]=max(v[18],v[20]);
            WRITES(cur3,"Minimum",v[21]);
           }
          else
           {
            v[22]=uniform(v[15],v[16]);
            WRITES(cur3,"Minimum",v[22]);
           }
         }

       }

     }
    if(v[1]>1)
     { // exectuives, above the first tier workers
      CYCLES(cur1, cur2, "Need")
       {
        CYCLES(cur2, cur3, "DCh")
         {
          v[23]=VS(cur3,"IdDCh");
          if(v[23]==1)
           {
            cur4=SEARCH_CNDS(cur,"NumClass",v[1]-1);
            CYCLES(cur4, cur5, "DCh")
             {
              v[24]=VS(cur5,"IdDCh");
              if(v[24]==1)
               v[25]=VS(cur5,"Minimum");
             }
            v[26]=v[25]*v[10];
            WRITES(cur3,"Minimum",v[26]);
           }
          else
           {
            v[27]=uniform(v[15],v[16]);
            WRITES(cur3,"Minimum",v[27]);
           }
         }

       }

     }
   }
 }

PARAMETER
RESULT(1 )


EQUATION("TauInit")
/*
Set the initial values for the preferences (tolerance toward difference from max char in the market) across classes, needs and characteristics
*/

v[1]=V("tauMinE"); // minimum level of tolerance toward the best product in the market, for engineers
v[2]=V("tauMaxE"); // maximum level of tolerance toward the best product in the market, for engineers
CYCLE(cur, "Demand")
 {
  CYCLES(cur, cur1, "Class")
   {
    v[6]=VS(cur1,"NumClass");
    if(v[6]<1)
     {// engineers
      CYCLES(cur1, cur2, "Need")
       {
        CYCLES(cur2, cur3, "DCh")
         {
          v[7]=uniform(v[1],v[2]);
          WRITES(cur3,"tau",v[7]);
         }

       }

     }
    if(v[6]==1)
     { // first tier workers
      CYCLES(cur1, cur2, "Need")
       {
        CYCLES(cur2, cur3, "DCh")
        {
          v[3]=VS(cur3,"tauMinW"); // minimum level of tolerance toward the best product in the market, for workers
          v[4]=VS(cur3,"tauMaxW"); // maximum level of tolerance toward the best product in the market, for wokers
          v[8]=uniform(v[3],v[4]);
          if(VS(cur3,"NegativeQuality")>0)
           v[8]=1-v[8];
          WRITES(cur3,"tau",v[8]);
         }

       }

     }
    if(v[6]>1)
     { // exectuives, above first tier
      CYCLES(cur1, cur2, "Need")
       {
        v[9]=VS(cur2,"IdNeed");
        CYCLES(cur2, cur3, "DCh")
         {
          v[10]=VS(cur3,"IdDCh");
          cur4=SEARCH_CNDS(cur,"NumClass",(v[6]-1));
          CYCLES(cur4, cur5, "Need")
           {
            v[11]=VS(cur5,"IdNeed");
            if(v[11]==v[9])
             { // check in the same need
              CYCLES(cur5, cur6, "DCh")
               { // and in the same characteristic
                v[12]=VS(cur6,"IdDCh");
                if(v[12]==v[10]) // if it is in the same characteristic and in the same need, in the previous class, take the value of tau
                 v[13]=VS(cur6,"tau");
               }

             }
           }
          v[5]=VS(cur3,"tauMultiplier"); // the speed of adjustment of the tolerance level from one class to the following one
          v[16]=VS(cur3,"NegativeQuality");
          if(v[16]>0)
           v[15]=V("tauMax"); // the asympthotic level of the tolerance level (1 for qualities and 0 for price)
          if(v[16]<0)
           v[15]=V("tauMin"); // the asympthotic level of the tolerance level (1 for qualities and 0 for price)
          v[14]=v[13]*(1-v[5])+v[5]*v[15]; // adjustment in the treshold level of tolerance
          WRITES(cur3,"tau",v[14]); // finally write the tau for the new consumer class in each of its characteristic for each need
          //INTERACTS(cur3, "tauini", v[13]);
         }

       }

     }
   }

 }

PARAMETER
RESULT(1 )


EQUATION("ShareInit")
/*
Set the initial distribution of shares (in the class above the first tier, engineers and firts tier are set exogenously)
*/

CYCLE(cur, "Demand")
 {
  CYCLES(cur, cur1, "Class")
   {
    v[1]=VS(cur1,"NumClass");
    if(v[1]>1)
     { // if the class is above the first tier of workers, compute the change in the expenditure shares with resepct to the first class
      v[2]=VS(cur1,"ComputeShare");
     }
   }

 }

PARAMETER
RESULT(1 )




/***** SOME STATISTICS ******/

EQUATION("MargPropConsume")
/*
Comment
*/
v[1]=v[2]=v[3]=v[4]=0;
CYCLE(cur, "Class")
 {
  v[5]=VS(cur,"Expenditure");
  v[6]=VLS(cur,"Expenditure",1);
  v[7]=VS(cur,"Income");
  v[8]=VLS(cur,"Income",1);
  v[1]+=v[5];
  v[2]+=v[6];
  v[3]+=v[7];
  v[4]+=v[8];
 }
v[9]=(v[1]-v[2])/(v[3]-v[4]);
RESULT(v[9] )



EQUATION("IncomeDistribution")
/*
Herfindahl Index for the Income
*/
v[2]=0;
v[3]=0;
v[4]=0;
CYCLE(cur, "Class")
 {
  v[5]=VS(cur,"ShareWageIncome");
  v[6]=VS(cur,"SharePremiaIncome");
  v[8]=v[5]+v[6];
  v[2]+=(v[8]*v[8]);
  v[3]+=(v[5]*v[5]);
  v[4]+=(v[6]*v[6]);
 }
WRITE("HerfTotalIncome",v[2]);
WRITE("HerfWageIncome",v[3]);
WRITE("HerfNonWageIncome",v[4]);
v[10]=V("TotWage");
v[11]=V("TotPremia");
v[12]=V("TotIncome");
v[13]=v[10]/v[12];
v[14]=v[11]/v[12];
WRITE("WageIncomeRatio",v[13]);
WRITE("PremiaIncomeRatio",v[14]);
RESULT(1 )

EQUATION("TopBotInc")
/*
Ratio between top class and bottom class income
*/
v[2]=0;
CYCLE(cur, "Class")
 {
  v[1]=VS(cur,"NumClass");
  if(v[1]>v[2])
   {
    v[2]=v[1];
    v[3]=VS(cur,"Income"); // Top class income
   }
  if(v[1]==2)
   v[4]=VS(cur,"Income"); //Bottom class income
 }
v[5]=v[3]/v[4];

RESULT(v[5])

EQUATION("TopBotEarn")
/*
Ratio between top and bottom class wages
*/

v[2]=0;
CYCLE(cur, "Class")
 {
  v[1]=VS(cur,"NumClass");
  if(v[1]>v[2])
   {
    v[2]=v[1];
    v[3]=VS(cur,"WageIncome"); //Top class earings
   }
  if(v[2]==2)
   v[4]=VS(cur,"WageIncome"); // Bottom class wage
 }
v[5] = v[3]/v[4];

RESULT(v[5] )


EQUATION("InvHerfIndex")
/*
Inverse Herfindahl Index for firm sales in each sector
*/
v[0]=0;
v[1]=0;
CYCLE(cur, "Firm")
 {
  v[2]=VS(cur,"UnitSales");
  v[0]+=v[2];
 }
WRITE("TotalSales",v[0]);
CYCLE(cur, "Firm")
 {
 v[3]=VS(cur,"UnitSales");
 //v[4]=v[3]/v[0];
 v[0]==0 ? v[4]=0 : v[4]=v[3]/v[0];
 v[1]+=(v[4]*v[4]);
 }
//v[5]=1/v[1];
v[1]==0 ? v[5]=0 : v[5]=1/v[1];
RESULT(v[5] )


EQUATION("Andre")
/*
Lorenz curve
*/

v[5]=v[10]=(double)t;
if(v[10]>1)
 {
  v[5]=v[6]=v[9]=0;
  v[4]=V("TotPremia");
  v[11]=V("TotWage");
  CYCLE(cur, "Demand")
   {
    SORTS(cur,"Class","ShareIncome", "UP");
    CYCLES(cur, cur1, "Class")
     {
      v[1]=VS(cur1,"ShareWageIncome");
      v[2]=VS(cur1,"SharePremiaIncome");
      v[12]=(v[1]*v[11])/(v[4]+v[11]);
      v[13]=(v[2]*v[4])/(v[4]+v[11]);
      v[3]=(v[12]+v[13]);
      WRITES(cur1,"Lorenz",v[3]+v[6]);
      v[6]=v[3]+v[6];
      v[5]+=v[3];
      v[8]=VS(cur1,"Individuals");
      v[9]+=v[8];
      WRITES(cur1,"LorenzInd",v[9]);
     }
    SORTS(cur,"Class","NumClass", "UP");

   }
 }

RESULT(v[5] )



EQUATION("TotIndividuals")
/*
Total number of consumers (workers) in t-1
*/

V("ShareIncome");
v[2]=0;
CYCLE(cur, "Demand")
 {
  CYCLES(cur, cur1, "Class")
   {
    v[1]=VS(cur1,"Individuals");
    v[2]+=v[1];
   }

 }

RESULT(v[2] )


EQUATION("AvIncome")
/*
Average income across classes in time t-1
*/

v[5]=(double)t;
if(v[5]>1)
 {
  V("ShareIncome");
  v[1]=V("TotIncome");
  v[3]=V("TotIndividuals");
  v[4]=(v[1])/v[3];
 }
else
 v[4]=1;

RESULT(v[4] )


EQUATION("Atkinson")
/*
Atkinson index of inequality for income in period t-1
A=1-{SUM_i[y_i**(1-e)]**(1/(1-e))}/Av.y
*/

v[16]=(double)t;
if(v[16]>1)
 {
  v[10]=v[20]=v[30]=v[40]=0;
  //v[6]=V("AvIncome");
  v[8]=V("Aversion"); // parmeter for the aversion to inequality (changes which end of the income distribution has a higher weight in the index computation)
  v[11]=V("TotIndividuals");
  v[22]=V("TotIncome");
  CYCLE(cur, "Demand")
   {
    CYCLES(cur, cur1, "Class")
     {
      v[4]=VS(cur1,"Individuals");
      v[5]=VS(cur1,"Income");
      if(v[5]>0 && v[4]>0.01)
       {
        v[7]=v[5]/v[4]; // av. income of the class
        v[9]=pow(v[7],(1-v[8]));
        v[17]=v[9]*v[4];
        v[41]=pow(v[5],(1-v[8]));
       }
      else
      	{
       v[17]=0;
       v[41]=0;
       	}
      v[10]+=v[17];
      v[20]+=v[5];
      v[30]+=1;
      v[40]+=v[41];
     }

   }

  v[12]=v[10]/v[11];
  v[13]=1/(1-v[8]);
  v[14]=pow(v[12],v[13]);
  v[15]=1-v[14]/(v[20]/v[11]);//Atkinson Index for Individual inequality
  v[42]=v[41]/v[30];
  v[43]=pow(v[42],v[13]);
  v[44]=1-v[43]/(v[20]/v[30]);//Atkinson Index for Individual inequality
  WRITE("AtkinsonClass",v[44]);
 }
else
 v[15]=0;

RESULT(v[15] )

EQUATION("Gini")
/*
Gini coefficient in t-1
*/

v[20]=(double)t;
if(v[20]>1)
{
CYCLE(cur, "Demand")
 {
  v[14]=v[21]=0;
  v[2]=VS(cur,"TotWage");
  v[3]=VS(cur,"TotPremia");

  CYCLES(cur, cur1, "Class")
   {
    v[1]=VS(cur1,"ShareIncome");
    if(v[1]>0)
     {
      v[22]=VLS(cur1,"ShareWageIncome",1);
      v[23]=VLS(cur1,"SharePremiaIncome",1);
      v[24]=v[22]*v[2];
      v[25]=v[23]*v[3];
      v[5]=VS(cur1,"Individuals");
      v[6]=(v[24]+v[25]); // total class income of the comparing class
      v[7]=VS(cur1,"NumClass");
      v[21]++;
      CYCLES(cur, cur2, "Class")
       {
        v[8]=VS(cur2,"NumClass");
        if(v[8]!=v[7])
         { // if it is a different class
          v[9]=VS(cur2,"ShareIncome");
          if(v[9]>0)
           {
            v[26]=VLS(cur2,"ShareWageIncome",1);
            v[27]=VLS(cur2,"SharePremiaIncome",1);
            v[28]=v[26]*v[2];
            v[29]=v[27]*v[3];
            v[11]=VS(cur2,"Individuals");
            v[12]=(v[28]+v[29]); // total class income of the compared class
            v[13]=abs(v[6]-v[12]);
            v[14]+=v[13];
           }
         }
       }

     }
   }

 }
v[15]=V("AvIncome");
v[30]=(v[2]+v[3])/v[21]; // average income per class
v[16]=V("TotIndividuals");
v[17]=pow(v[21],2);
v[18]=2*v[17]*v[30];
v[19]=v[14]/v[18];
}
else
 v[19]=0;

RESULT(v[19] )


EQUATION("GDPnom")
/*
Nominal GDP, at varying prices
*/

v[4]=0;
v[8]=v[9]=v[10]=v[30]=v[31]=0;
CYCLE(cur, "Supply")
 {
  CYCLES(cur, cur1, "Firm")
   {
    v[1]=VS(cur1,"Revenues");
    v[4]+=v[1];
    v[5]=VS(cur1,"UnitSales");
    v[6]=VS(cur1,"ConstPrice");
    v[7]=v[5]*v[6];
    v[9]+=v[7];
   }

 }
CYCLE(cur, "Machinery")
 {
  CYCLES(cur, cur1, "KFirm")
   {
    v[5]=VS(cur1,"KProductionFlow");
    v[6]=VS(cur1,"KPrice");
    v[11]=VS(cur1,"KConstPrice");
    v[8]+=v[6]*v[5];
    v[10]+=v[11]*v[5];
    v[30]+=0.3333*v[11]/v[6]; //deflator
   }

 }
v[40] = V("SumSumKOandMExpenditure");
v[41] = V("SumCapitalPaymentEF");
v[42] = V("SumCapitalPaymentF");
v[43]= v[40]+v[41]+v[42];//nominal K gdp

v[44]=v[43]*v[30]; //real K gdp
WRITE("KGDPsmooth", v[43]);
WRITE("KGDPspike",v[8]);

v[20]=V("PriceEN");
v[21]=V("TotEnergyConsumption");
v[22]=v[20]*v[21];

WRITE("GDPl",V("GDP"));

v[11]=v[9]+v[44];
v[12]=v[4]+v[43];
if(v[12]==0)
	v[13]=0;
else
	v[13]=1/v[12];
v[14]=v[4]*v[13];
v[15]=v[43]*v[13];

v[30]=v[22]*v[13];

v[50] = V("GDP");
//if(v[11]/v[50] > 2)
 //INTERACT("Che GDP!", v[50]);
WRITE("GDP",v[11]);
WRITE("GDPF",v[9]);
WRITE("GDPK",v[44]); // Proxy for investment at constant prices
WRITE("ConsumptionGdpRatio",v[14]);
WRITE("InvestmentGdpRatio",v[15]);
WRITE("EnergyGdpRatio",v[30]);

RESULT(v[12] )






EQUATION("GDPnomg")
/*
Growth rate of the nominal GDP
*/

v[4]=(double)t;
if(v[4]>2)
 {
  v[1]=VL("GDPnom",1);
  v[2]=V("GDPnom");
  v[3]=(v[2]/v[1])-1;
 }
else
 v[3]=0;

RESULT(v[3] )


EQUATION("GDPg")
/*
Growth rate of the GDP at constant prices
*/

//v[4]=(double)t;
v[4]=t;

if(v[4]>2)
 {
   v[1]=V("GDPl");
  //v[1]=VL("GDP",1);
  //INTERACT("GdPC_1",v[1]);
  v[2]=V("GDP");
  //INTERACT("GdPC",v[2]);
  v[1]==0 ? v[3]=0 : v[3]=v[2]/v[1]-1;
  //INTERACT("GrowthC",v[3]);

 }
else
 v[3]=0;

RESULT(v[3] )


EQUATION("MoAvGDPc")
/*
Growth rate of the GDP at constant prices
*/

//v[4]=(double)t;

  v[1]=V("GDPg");
  //INTERACT("MoAvGDPc",v[1]);
  v[0]=VL("MoAvGDPc",1);
  //INTERACT("GDPg1",v[0]);
  v[2]=0.9*v[0]+0.1*v[1];
  //INTERACT("MoAv",v[2]);

RESULT(v[2] )



EQUATION("TotInventories")
/*
Total inventories in the economy, and their growth
*/

//v[4]=(double)t;
//v[4]=(double)t;
v[4]=t;

v[1]=v[2]=0;
CYCLE(cur, "Supply")
 {
  CYCLES(cur, cur1, "Firm")
   {
    v[1]+=VS(cur1,"Stocks");
    v[2]+=VLS(cur1,"Stocks",1);
   }

 }
if(v[4]>2)
 v[3]=v[1]/v[2]-1;
else
 v[3]=0;
//WRITE("InventGrowth",v[3]);

RESULT(v[1] )

EQUATION("InvSalesRatio")
/*
ratio between inventories and sales in the final good sector
*/

v[1]=V("TotInventories");
v[2]=V("TotUSales");

v[2]==0 ? v[0]=0 : v[0]=v[1]/v[2];

RESULT(v[0])




EQUATION("AvProfit")
/*
Average profits across firms
*/

v[2]=v[3]=0;
CYCLE(cur, "Supply")
 {
  CYCLES(cur, cur1, "Firm")
   {
    v[1]=VS(cur1,"Profit");
    v[2]+=v[1];
    v[3]++;
   }

 }
v[4]=v[2]/v[3];

RESULT(v[4] )


EQUATION("AvKProfit")
/*
Average of the capital firms profits
*/

v[2]=v[3]=0;
CYCLE(cur, "Machinery")
 {
  CYCLES(cur, cur1, "KFirm")
   {
    v[1]=VS(cur1,"KProfit");
    v[2]+=v[1];
    v[3]++;
   }

 }
v[4]=v[2]/v[3];

RESULT(v[4] )

EQUATION("Avx")
/*
Average value of the quality characteristic (non price characteristic that is not price)

Compute also all the statistics for sectors.
*/

v[3]=v[4]=0;

CYCLE(cur, "Sectors")
 {
  VS(cur,"Demography");
  WRITES(cur,"AvxS",0);
  WRITES(cur,"SUnitSales",0);
  WRITES(cur,"SQ",0);
  WRITES(cur,"SRevenues",0);
  WRITES(cur,"SProfits",0);
  WRITES(cur,"SNumFirms",0);
  WRITES(cur,"SMonetarySales",0);
  WRITES(cur,"Sapp",VS(cur,"maxXS"));
  WRITES(cur,"maxXS",0);
  WRITES(cur,"AvpS",0);
  WRITES(cur,"SInvHerf",0);
  WRITES(cur,"SAvHealth",0);
  WRITES(cur,"SAvStock",0);
  WRITES(cur,"SAvBacklog",0);
  WRITES(cur,"SKProductivity",0);
  WRITES(cur,"SULC",0);
  WRITES(cur,"SnumBLI",0);
  WRITES(cur,"SNetWorth",0);
  WRITES(cur,"SAvAge",0);
  WRITES(cur,"SBacklogShare",0);

 }

CYCLE(cur, "Supply")
 {
  CYCLES(cur, cur1, "Firm")
   {
    INCRS(cur1->hook->up, "SBacklogShare", VS(cur1,"backlog"));
    INCRS(cur1->hook->up, "SAvAge", VS(cur1,"Age"));
    INCRS(cur1->hook->up, "SUnitSales", VS(cur1,"UnitSales"));
    INCRS(cur1->hook->up, "SnumBLI", VS(cur1,"numBLI"));
    INCRS(cur1->hook->up, "SNetWorth", VS(cur1,"NetWorth"));
    INCRS(cur1->hook->up, "SQ", v[30]=VS(cur1,"Q"));
    INCRS(cur1->hook->up, "SRevenues", VS(cur1,"Revenues"));
    INCRS(cur1->hook->up, "AvpS", v[30]*VS(cur1,"price"));
    INCRS(cur1->hook->up, "SProfits", VS(cur1,"Profit"));
    INCRS(cur1->hook->up, "SMonetarySales", VS(cur1,"MonetarySales"));
    INCRS(cur1->hook->up, "SNumFirms", 1);
    WRITES(cur1,"MsSector",VS(cur1, "MonetarySales"));
    INCRS(cur1->hook->up, "SKProductivity", VS(cur1,"MaxLaborProductivity")*v[30]);
    INCRS(cur1->hook->up, "SULC", VS(cur1,"UnitLaborCost")*v[30]);

    CYCLES(cur1, cur2, "PNeed")
     {
      CYCLES(cur2, cur3, "Ch")
       {
        v[1]=VS(cur3,"IdCh");
        if(v[1]>1)
         { // to make it simple i assume that the first charateristic is price, this should be generalised
          v[2]=VS(cur3,"x");
          INCRS(cur1->hook->up, "AvxS", v[2]*v[30]);
          v[5]=VS(cur1->hook->up,"maxXS");
          if(v[2]>v[5])
           WRITES(cur1->hook->up,"maxXS",v[2]);
          v[3]+=v[2];
          v[4]++;
         }
       }

     }

   }

 }

v[40]=v[41]=0;
CYCLE(cur1, "Firm")
 {
  v[20]=VS(cur1->hook->up,"SMonetarySales");
  v[21]=VS(cur1,"MsSector");

  if(v[20]>0)
   v[23]=v[21]/v[20];
  else
   v[23]=0;
  WRITES(cur1,"MsSector",v[23]);
  INCRS(cur1->hook->up,"SAvHealth",v[23]*VS(cur1,"Health"));
  INCRS(cur1->hook->up,"SInvHerf",v[23]*v[23]);
  INCRS(cur1->hook->up,"SAvStock",v[23]*VS(cur1,"Stocks"));
  INCRS(cur1->hook->up,"SAvBacklog",v[23]*VS(cur1,"backlog"));
  v[42]=VS(cur1,"LaborCost");
  v[43]=VS(cur1,"LaborForce");
  v[40]+=v[42];
  v[41]+=v[43];
 }
if(t>1)
 v[44]=V("TotAvWage");
else
 v[44]=v[40]/v[41];
WRITE("TotAvWage",v[40]/v[41]);
v[45]=(v[40]/v[41]-v[44])/v[44];
WRITE("WageGrowth",v[45]);

CYCLE(cur1, "Sectors")
 {
  v[27]=VS(cur1,"SInvHerf");
  if(v[27]>0)
   WRITES(cur1,"SInvHerf",1/v[27]);
  v[30]=VS(cur1,"SQ");
  if(v[30]>0)
   {
    MULTS(cur1,"AvxS",1/v[30]);
    MULTS(cur1,"SKProductivity",1/v[30]);
    MULTS(cur1,"SULC",1/v[30]);
    MULTS(cur1,"AvpS",1/v[30]);
    MULTS(cur1,"SBacklogShare",1/v[30]);

   }
  else
   {
    WRITES(cur1,"maxXS",VS(cur1,"Sapp"));
   }
  v[17]=VS(cur1,"SNumFirms");
  if(v[17]>0)
   {
    MULTS(cur1,"SnumBLI",1/v[17]);
    MULTS(cur1,"SNetWorth",1/v[17]);
    MULTS(cur1,"SAvAge",1/v[17]);
   }
 }

v[5]=v[3]/v[4];

RESULT(v[5] )

EQUATION("Dtau")
/*
Difference between the minimum and the maximum asymptoti level of tau: gives the theoretical heterogeneity in consuemrs preferces, while the standard deviaiton depends on how many tiers develop through time (as it is affected by the speed of change)
*/

v[1]=V("tauMax");
v[2]=V("tauMin");
v[3]=v[1]-v[2];

RESULT(v[3] )


EQUATION("HerfIndexS")
/*
Herfindahl indexin each sector
*/

v[4]=v[8]=0;
v[1]=V("IdGood");
CYCLES(p->up, cur, "Supply")
 {
  CYCLES(cur, cur1, "Firm")
   {
    v[2]=VS(cur1,"product");
    if(v[2]==v[1])
     {
      v[3]=VS(cur1,"UnitSales");
      v[4]+=v[3];
     }
   }

 }
WRITE("TotSalesS",v[4]);
CYCLES(p->up, cur, "Supply")
 {
  CYCLES(cur, cur1, "Firm")
   {
    v[5]=VS(cur1,"product");
    if(v[5]==v[1])
     {
      v[6]=VS(cur1,"UnitSales");
      v[7]=v[6]/v[4];
      WRITES(cur1,"MsSector",v[7]);
      v[8]+=(v[7]*v[7]);
     }
   }

 }

RESULT(v[8] )

EQUATION("SectEmployment")
/*
Herfindahl indexin each sector
*/

v[0]=v[10]=v[20]=v[30]=0;
v[1]=V("IdGood");
v[4]=V("NbrWorkers");
v[14]=V("GDPnom");
v[24]=V("GDP");
  CYCLE( cur1, "sFirm")
   {
      v[3]=VS(cur1->hook,"NumWorkers");
      v[11]=VS(cur1->hook,"Revenues");
      v[21]=VS(cur1->hook,"UnitSales");
      v[22]=VS(cur1->hook,"ConstPrice");
      v[31]=VS(cur1->hook,"CapitalStock");
      v[23]=v[21]*v[22];
      v[0]+=v[3];
      v[10]+=v[11];
      v[20]+=v[23];
      v[30]+=v[31];
   }

v[7]=v[0]/v[4];
v[17]=v[10]/v[14];
v[24]==0 ? v[27]=0 : v[27]=v[20]/v[24];
WRITE("SectEmploymentShare",v[7]);
WRITE("SectSales",v[10]);
WRITE("SectNomGDPShare",v[17]);
WRITE("SectOutputShare",v[27]);
v[32]=v[30]/v[0];
WRITE("SectMechanisation",v[32]);
RESULT(v[0] )

EQUATION("ConsumptionConcentration")
/*
Comment
*/
v[0]=v[10]=0;
CYCLE(cur, "Sectors")
 {
 v[1]=VS(cur,"SectSales");
 v[0]+=v[1];
 }
v[2]=v[0];
CYCLE(cur, "Sectors")
 {
 v[1]=VS(cur,"SectSales");
 if(v[2]==0)
 	v[3]=0;
 else
	 v[3]=v[1]/v[2];
 WRITES(cur,"SectSalesShare",v[3]);
 v[10]+=(v[3]*v[3]);
 }
if(v[10]==0)
	v[11]=0;
else
	v[11]=1/v[10];
RESULT(v[11] )

EQUATION("Mechanisation")
/*
Comment
*/
v[0]=0;
CYCLE(cur, "Sectors")
 {
 v[1]=VS(cur,"SectMechanisation");
 v[2]=VS(cur,"SectEmploymentShare");
 v[0]+=(v[1]*v[2]);
 }
v[3]=v[0];
RESULT(v[3] )

EQUATION("EmploymentConcentration")
/*
Comment
*/
v[0]=v[3]=0;
v[4]=V("NbrWorkers");
v[10]=v[11]=v[12]=0;
v[13]=V("GDPnom");
v[20]=v[21]=v[22]=0;
v[23]=V("GDP");
CYCLE(cur, "Sectors")
 {
 	v[5]=VS(cur,"SectEmploymentShare");
 	v[0]+=(v[5]*v[5]);
 	v[14]=VS(cur,"SectNomGDPShare");
 	if(v[14]==0)
		v[19]=0;
	else
		v[19]=v[14]*v[14];
	v[10]+=v[19];
 	v[24]=VS(cur,"SectOutputShare");
 	if(v[24]==0)
		v[29]=0;
	else
		v[29]=v[24]*v[24];
	v[20]+=v[29];
}

CYCLE(cur, "Machinery")
 {
 	v[1]=v[2]=0;
  CYCLES(cur, cur1, "KFirm")
   {
    CYCLES(cur1, cur2, "KLabor")
     {
      v[6]=VS(cur2,"KNbrWorkers");
      v[2]+=v[6];
     }
		CYCLES(cur1, cur2, "KEngineers")
 		{
  		v[7]=VS(cur2,"KNbrEngineers");
      v[1]+=v[7];
 		}
	  v[15]=VS(cur1,"KProductionFlow");
    v[16]=VS(cur1,"KPrice");
    v[11]+=(v[15]*v[16]);
    v[26]=VS(cur1,"KConstPrice");
    v[21]+=v[15];
   }
	v[8]=v[1]+v[2];
	v[9]=v[8]/v[4];
	v[3]+=(v[9]*v[9]);
	WRITES(cur,"KEmployment",v[8]);
	WRITES(cur,"KEmploymentShare",v[9]);
	v[17]=v[11]/v[13];
	if(v[11]==0)
		v[18]=0;
	else
		v[18]=v[17]*v[17];
	v[12]+=v[18];
	WRITES(cur,"KNomGDPShare",v[17]);
		//v[27]=v[21]/v[23];
		v[23]==0 ? v[27]=0 : v[27]=v[21]/v[23];
	if(v[21]==0)
		v[28]=0;
	else
		v[28]=v[27]*v[27];
	v[22]+=v[28];
	WRITES(cur,"KOutputShare",v[27]);
 }
v[99]=1/(v[3]+v[0]);
v[98]=v[12]+v[10];
if(v[98]==0)
	v[97]=0;
else
	v[97]=1/v[98];
WRITE("GDPConcentration",v[97]);
v[96]=v[22]+v[20];
if(v[96]==0)
	v[95]=0;
else
	v[95]=1/v[96];
WRITE("OutputConcentration",v[95]);
RESULT(v[99])




EQUATION("FrontierX")
/*
Imitable value of X for new entrants
*/
//DEBUG_START;

V("AvxS");
v[0]=V("maxXS");

v[1]=V("aFrontierX");

v[3]=CURRENT;

//INTERACT("current",v[3]);


v[2]=v[1]*CURRENT+(1-v[1])*v[0];
if(v[2]==0)
	INTERACT("ZeroFrontierX",v[2]);

//DEBUG_STOP;
RESULT(v[2] )



EQUATION("Dump")
/*
Dumping data for inequality study
*/

if(t!=100 && t%1000!=0)
 END_EQUATION(0);
 //sprintf(msg, "data%d.txt", t);
//f=fopen(msg, "w");

fprintf(f,"Num\tIncome\tCons\tWealth\n");
cur2=SEARCH("Supply");

CYCLES(cur2, cur, "Firm")
 {
  CYCLES(cur, cur1, "Labor")
   {
    v[0]=VS(cur1,"NumWorkers");
    v[1]=VS(cur1,"wage");
    v[2]=V("Premia");
    v[3]=VS(cur1->hook,"Consumption");
    v[4]=VS(cur1->hook,"Individuals");
    v[5]=VS(cur1->hook,"BalanceC");
    fprintf(f, "%lf\t%lf\t%lf\t%lf\n", v[0],v[1]+v[2],(v[3]/v[4])*v[0],(v[5]/v[4])*v[0]);
   }

 }

fclose(f);
RESULT(1 )


/********************************************************************************************

STATISTICS OVER THE RANDOM RUNS

********************************************************************************************/

EQUATION("Init_Init")
/*
Requires first to initialise all the simulation runs
*/
CYCLE(cur, "Country")
 {
  v[0]=VS(cur,"InProgress");
  if(v[0]==0)
    VS(cur,"Init");
  else
    VS(cur,"InitInProgress");
 }
PARAMETER
RESULT(1 )


// AVERAGES



EQUATION("Av_Computations")
/*
Activate the equation for income distribution HI, Lorenz curve
*/

CYCLE(cur, "Country")
 {VS(cur,"AvProfit");
  VS(cur,"Trade");
  cur1=SEARCHS(cur,"Demand");
  VS(cur1,"IncomeDistribution");
  VS(cur,"Production");
  VS(cur,"Andre");
 }

RESULT(1 )



EQUATION("TestFinance")
/*
Comment
*/
v[0]=v[1]=v[2]=v[3]=v[4]=v[5]=0;

CYCLE(cur, "Firm")
 {
  v[0]+=VS(cur,"DebtF");//xxx
  v[1]+=VS(cur,"BalanceF");

 }
CYCLE(cur, "KFirm")
 {
  v[0]+=VS(cur,"DebtK");//xxx
  v[1]+=VS(cur,"BalanceK");
 }

v[10]=V("TotalCapital");



RESULT(1 )


EQUATION("InitInProgress")
/*
Initialization function establishing only the hooks and leaving numbers of objects and values as they are, supposedly generated by a concluded simulation run with standard init (i.e. param InProgress=0).
*/

cur8=SEARCH("Bank");
cur7=SEARCH_CND("NumClass",0);

CYCLE(cur, "Firm")
 {
  v[0]=VS(cur,"product");
  cur1=SEARCH_CND("IdGood",v[0]);
  cur->hook=cur1;
  v[2]=VS(cur,"IdFirm");
  cur3=SEARCH_CNDS(cur1,"IdsFirm",v[2]);
  cur->hook=cur3;
  cur3->hook=cur;
  //v[6]=VLS(cur,"price",1);
  //WRITES(cur,"ConstPrice",v[6]);
  v[7]=VS(cur,"LaborForce");
  WRITES(cur,"LaborForceL",v[7]);
  CYCLES(cur, cur1, "Labor")
   {
    v[1]=VS(cur1,"IdLabor");
    cur2=SEARCH_CND("NumClass",v[1]);
    cur1->hook=cur2;

   }
  v[31]=0;
  CYCLES(cur,cur1, "Capital")
  {
      v[31]+=VS(cur1, "K");
      WRITELLS(cur1, "KAge", 1, t-1,t-1);
  }

  WRITELLS(cur, "CapitalStock",v[31],t-1,t-1);
  cur3=SEARCHS(cur,"BankF");
  cur3->hook=cur8;

  cur7=SEARCH_CND("NumClass",0);
  cur3=SEARCHS(cur,"Engineers");
  cur3->hook=cur7;


 }

CYCLE(cur, "Class")
 {
  CYCLES(cur, cur1, "Need")
   {
    v[3]=VS(cur1,"IdNeed");
    cur2=SEARCH_CND("IdGood",v[3]);
    cur1->hook=cur2;
   }
  cur1=SEARCHS(cur,"BankC");
  cur1->hook=cur8;
 }

CYCLE(cur, "KFirm")
 {
  //v[7]=VLS(cur,"KPrice",1);
  //WRITES(cur,"KConstPrice",v[7]);
  CYCLES(cur, cur1, "KLabor")
   {
    v[4]=VS(cur1,"IdKLabor");
    cur2=SEARCH_CND("NumClass",v[4]);
    cur1->hook=cur2;
    
   }
  cur3=SEARCHS(cur,"KEngineers");
  cur3->hook=cur7;
  if(VS(cur, "NumOrders")>0)
  {
  CYCLES(cur, cur1, "Order")
   {
      v[5]=VS(cur1,"IdClient");
      if(VS(cur1,"EnergyKOrder")==0)
       cur3=SEARCH_CND("IdFirm",v[5]);
      else
       cur3=SEARCH_CND("IdFirmEN",-v[5]);
      cur1->hook=cur3;
   }
  }
  else
  {v[40]=0;
  CYCLES(cur, cur1, "Order")
   {v[40]++;
    cur1->hook=NULL;
    WRITES(cur1, "IdClient", -1);
   }
  }

  cur1=SEARCHS(cur,"BankK");
  cur1->hook=cur8;

 }


V("TauInit");
V("ShareInit");

v[40]=0;


CYCLE(cur1,"EnergyFirm")
	{
	CYCLES(cur1,cur, "LaborEF")
  	{
  	 v[41] = VS(cur, "IdLaborEF");
    cur2 = SEARCH_CND("NumClass", v[41]);
    cur->hook=cur2;
    
    
  	}
	}
V("InitPP");

RESULT( 1)


EQUATION("ControlWaiting")
/*
Control the matching between parameter waiting and capital orders
*/
cur1=SEARCHS(p->up, "Machinery");
CYCLE(cur, "Firm")
{
	v[0] = VS(cur, "Waiting");
	  v[1] = VS(cur, "IdFirm");
	  //LOG("\n %d", (int)v[1]);
	  //plog(msg);
	  v[3]=0;
	  CYCLES(cur1, cur3, "KFirm")
	  {
	  	CYCLES(cur3, cur4, "Order")
	  	{
	  		v[2] = VS(cur4, "IdClient");
	  		if(v[2]==v[1])
	  		 v[3]++;
	  	}

	  }
  if(v[3]>1)
    INTERACTS(cur, "Excess K",v[3]);
  if(v[0]==0 && v[3]>0)
    INTERACTS(cur, "Ghost K", v[3]);

}

RESULT(1 )


EQUATION("GlobalFlows")
/*
Check the global flows among modules of the model
At this time ensures that wages and premia paid by firms match the incomes received by the consumer classes.
*/

V("Production");
V("FinancialTrading");
V("Demography");
//V("DemographyEN");

v[0]=v[1]=v[2]=v[3]=v[4]=v[5]=v[6];

CYCLE(cur, "Class")
{
	WRITES(cur, "appCtrlWage", 0);
	WRITES(cur, "appCtrlPremia", 0);
}

CYCLE(cur, "Firm")
{
 if(VS(cur, "Age")>0)
 {
	CYCLES(cur, cur1, "Labor")
	{
		v[20] = VS(cur1, "wage");
		v[21] = VS(cur1, "NumWorkers");
    v[22] = VS(cur1, "Premia");
    if(cur1->hook==NULL)
     INTERACTS(cur1, "null class", v[22]);
   		INCRS(cur1->hook->up, "appCtrlWage", v[20]*v[21]);
		INCRS(cur1->hook->up, "appCtrlPremia", v[22]);

	}
 cur2 = SEARCHS(cur, "Engineers");
 v[23] = VS(cur2, "NbrEngineers");
 v[24] = VS(cur2, "WageEngineers");
     if(cur2->hook==NULL)
     INTERACTS(cur2, "null class", v[23]);

 INCRS(cur2->hook->up, "appCtrlWage", v[23]*v[24]);
 }
}

CYCLE(cur, "KFirm")
{
	CYCLES(cur, cur1, "KLabor")
	{
		v[20] = VS(cur1, "KWage");
		v[21] = VS(cur1, "KNbrWorkers");
    v[22] = VS(cur1, "KPremia");
		INCRS(cur1->hook->up, "appCtrlWage", v[20]*v[21]);
		INCRS(cur1->hook->up, "appCtrlPremia", v[22]);
	}
 cur2 = SEARCHS(cur, "KEngineers");
 v[23] = VS(cur2, "KNbrEngineers");
 v[24] = VS(cur2, "KWageEngineers");
  if(cur2->hook==NULL)
     INTERACTS(cur2, "null class", v[23]);
 INCRS(cur2->hook->up, "appCtrlWage", v[23]*v[24]);

}

CYCLE(cur, "EnergyFirm")
{
	CYCLES(cur, cur1, "LaborEF")
	{
		v[20] = VS(cur1, "wageEF");
		v[21] = VS(cur1, "NumWorkersEF");
    v[22] = VS(cur1, "PremiaEF");
     if(cur1->hook==NULL)
     INTERACTS(cur1, "null class", v[23]);
		INCRS(cur1->hook->up, "appCtrlWage", v[20]*v[21]);
		INCRS(cur1->hook->up, "appCtrlPremia", v[22]);
	}

}

v[29]=v[30]=0;

v[40] = V("Exit");
CYCLE(cur, "Class")
{
	v[25] = VS(cur, "appCtrlWage");
	v[26] = VS(cur, "WageIncome");
	/*
	if(abs(v[25]-v[26])>10)
	 INTERACTS(cur,"CtrlWage", v[25]-v[26]);
	*/
	v[27] = VS(cur, "PremiaIncome");
	v[28] = VS(cur, "appCtrlPremia");
	/*
	if(abs(v[28]-v[27])>10)
	 INTERACTS(cur,"CtrlPremia", v[28]-v[27]);
  */
	v[29]+=v[25]-v[26];
	v[30]+=v[28]-v[27];
	if(v[40]==0 && abs(v[25]-v[26])>10)
	 INTERACTS(cur, "Wrong CtrlWage", v[25]-v[26]);
}

RESULT(v[29]+v[30] )


EQUATION("SumKENCost")
/*
Comment
*/
v[0]=0;
CYCLE(cur, "KFirm")
{
	v[0] += VS(cur,"KENCost");
}

RESULT(v[0] )


EQUATION("CtrlEnergy")
/*
Comment
*/
v[0] = V("SumENCost");
v[1] = V("SumKENCost");
v[2] = V("SumRevenuesEF");
v[3]=abs((v[0]+v[1]-v[2])/v[2]);

if(v[3]>0.00001)
 INTERACT("Error FinEnergy", v[3]);

RESULT(v[0]+v[1]-v[2] )


MODELEND



void close_sim(void)
{

}




/*

Notes Tum

- Why does the order of preference make the differences the way in whch the code is now? firms below the threshold are thrown cycling through any of the carachteristic. Got it, we actually need to define a truncation of the characteristics browsing to allow for the choice of specific goods.

- We need to insert a random order of characteritic in some INIT equation

- In the TTB we do not allow for a distribution of needs in a class: acounted for by the 'share' parameter, exogenous
*/


/*

Notes, just to not forget:

- ProdCost should be smoothed in order to avoid sudden movements of prices

- Among the characteristics of product should be included: visibility (i.e. smoothed market shares+marketing) and price (to be inverted, probably quite a mess).



*/
