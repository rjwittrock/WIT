#ifndef LGFR_SCENARIO_SCHDDEST_H
#define LGFR_SCENARIO_SCHDDEST_H
// RW2STL -- inserted:
#include <string>
// End of RW2STL -- inserted includes.

#include <scenario/src/schdDesc.h>
#include <scenario/src/customer.h>

// Schedule description strings

// Note: Schedule descriptor abbreviations must be less than 
// LgFrScheduleDescriptor.maxAbbrevLength() characters long.

// LgFrScheduleFactory::abbreviationOf() can be used to get the abbreviation
// for a descriptor.

// When new descriptors are added, the appropriate schedule-factory classes
// initHashTable method has to get modified.

// Schedule description strings for commonly used schedules
// --------------------------------------------------------
// Part Schedules
const LgFrScheduleDescriptor LGFRCriticalPartRankingList(
                                                         "Critical Part Rank List",
                                                         "Crit Rank");
const LgFrScheduleDescriptor LGFRCriticalPartSupplyVolume ( "Critical Part Supply Volume",
                                                            "Crit Sup");
const LgFrScheduleDescriptor LGFRExcessVolume ( "Excess Volume",
                                                "Excess",
                                                "",
                                                "ּޮ�خ�");
const LgFrScheduleDescriptor LGFRFocussedShortageSchedule ( "Focussed Shortage Schedule Volume",
                                                            "Short Vol");
const LgFrScheduleDescriptor LGFRFSSProductionVolume ( "Focussed Shortage Schedule Production Volume",
                                                       "FSS Production");
const LgFrScheduleDescriptor LGFRProductionVolume ( "Production Volume",
                                                    "Production",
                                                    "",
                                                    "����خ�");
const LgFrScheduleDescriptor LGFRTotalPartDemandVol ( "Total Part Demand Volume",
                                                    "Total Demand");
const LgFrScheduleDescriptor LGFRRequirementVolume ( "Requirement Volume",
                                                     "Net Req",
                                                     "",
                                                     "MRP��ֳخ�");
const LgFrScheduleDescriptor LGFRSupplyVolume ( "Supply Volume",
                                                "Supply",
                                                "",
                                                "������خ�");
const LgFrScheduleDescriptor LGFRUsageVolume ( "Usage Volume",
                                                "Usage",
                                                "",
                                                "�ֳخ�");
const LgFrScheduleDescriptor LGFRCycleTime ( "Cycle Time",
                                             "Cycle Time");
const LgFrScheduleDescriptor LGFRFrozenZone ( "Frozen Zone",
                                              "Frozen Zone");
#if CUSTOMER_NUMBER == SCEDEMOS_NUMBER
const LgFrScheduleDescriptor LGFRStockVolume ( "Stock Volume",
                                               "Proj OnHand");
#else
const LgFrScheduleDescriptor LGFRStockVolume ( "Stock Volume",
                                               "Stock",
                                               "",
                                               "�޲�خ�");
#endif

const LgFrScheduleDescriptor LGFRScrapVolume ( "Scrap Volume",
                                               "Scrap");
const LgFrScheduleDescriptor LGFRTotalShipmentVol      ( "Total Shipment Vol",
                                                         "Tot Ship");
const LgFrScheduleDescriptor LGFRCapacitySupplyVolume("Capacity Supply Volume",
						      "Supply",
                                                      "",
						      "ɳخ�");
const LgFrScheduleDescriptor LGFRMaterialSupplyVolume("Material Supply Volume",
						      "Supply",
                                                      "",
						      "������خ�");
const LgFrScheduleDescriptor LGFRSupplyVolumeTimesUnitCost ( "Supply Volume Times Unit Cost",
                                                              "Supply Cost");
const LgFrScheduleDescriptor LGFRRequirementVolumeTimesUnitCost( "Requirement Volume Times Unit Cost",
								 "Net Req Cost");
const LgFrScheduleDescriptor LGFRStockVolumeTimesUnitCost( "Stock Volume Times Unit Cost",
							      "Stock Cost",
                                                              "",
							      "�޲�خ�");

// Demand Schedules
const LgFrScheduleDescriptor LGFRCumDemandVolume ( "Cum Demand Volume",
                                                   "Cum Dmd Qty");
const LgFrScheduleDescriptor LGFRDemandVolume ( "Demand Volume",
                                                "Dmd Qty",
                                                "",
                                                "�����");
const LgFrScheduleDescriptor LGFRShipmentVolume ( "Shipment Volume",
                                                  "Ship Qty",
                                                  "",
                                                  "ɳ���ĳ");
const LgFrScheduleDescriptor LGFRCumShipmentVolume ( "Cum Shipment Volume",
                                                     "Cum Ship");
const LgFrScheduleDescriptor LGFRBacklogVolume ( "Backlog Volume",
                                                 "Backlog",
                                                 "",
                                                 "ɳ����");
const LgFrScheduleDescriptor LGFRPriority ( "Priority",
                                            "Priority",
                                            "",
                                            "ճ����");
const LgFrScheduleDescriptor LGFRObj1ShipReward ( "Shipment Revenue",
                                                  "Ship Rev",
                                                  "",
                                                  "���޲���");


// Following are descriptors for CMRP
// -------------------------------------
//        Things for the "MRP-managed" (procureable)  MultiPartSchedule
const LgFrScheduleDescriptor LGFRMRPManagedSupplyVol ( "MRP Managed Supply Vol",
                                                       "Supply",
                                                       "",
                                                       "������خ�");
const LgFrScheduleDescriptor LGFRMRPManagedRequiredVol (   "MRP Managed Explosion Net Required Vol",
                                                           "Expl Net Req",
                                                           "",
                                                           "MRP��ֳخ�");


const LgFrScheduleDescriptor LGFRMRPManagedTotalPartDemandVol (   "MRP Managed Independent Demand",
                                                       "Indep Demand");
const LgFrScheduleDescriptor LGFRMRPManagedConsVol (   "MRP Managed Explosion Part Consumption",
                                                       "Expld Consumed");
const LgFrScheduleDescriptor LGFRMRPManagedTotalConsVol (   "MRP Managed Totoal Explosion Part Consumption",
                                                       "Tot Exp Cons");
const LgFrScheduleDescriptor LGFRMRPManagedCumSupplyVol (  "MRP Managed Cum Supply Vol",
                                                           "Cum Sup");
const LgFrScheduleDescriptor LGFRMRPManagedCumRequiredVol ("MRP Managed Cum Required Vol",
"Cum Expl NetReq");
const LgFrScheduleDescriptor LGFRMRPManagedExcessVol (     "MRP Managed Excess Vol",
                                                           "Excess");
const LgFrScheduleDescriptor LGFRMRPManagedFocussedShortageSchedule
					(     "MRP Managed Focussed Shortage Vol",
					      "Short Vol",
                                              "",
					      "̿�خ�");
const LgFrScheduleDescriptor LGFRMRPManagedStockVol (     "MRP Managed Stock Vol",
                                                          "Stock",
                                                          "",
                                                          "�޲�خ�");
const LgFrScheduleDescriptor LGFRMRPManagedScrapVol (     "MRP Managed Scrap Vol",
                                                          "Scrap");
const LgFrScheduleDescriptor LGFRMRPManagedUsageVol (     "MRP Managed Usage Vol",
                                                          "Usage",
                                                          "",
                                                          "�ֳخ�");
// Following one is incorrect, "Procurement Order Vol" is what is needed.
// const LgFrScheduleDescriptor LGFRProcurementReceiptVol =   "Procurement Receipt Vol";
// const std::string              LGFRProcurementReceiptVolAbbrev = "Proc Recpt";
const LgFrScheduleDescriptor LGFRProcurementVol (          "MRP Managed Implosion Net Required Vol",
                                                           "Implode Net Req",
                                                           "",
                                                           "PRM��ֳخ�");
const LgFrScheduleDescriptor LGFRProcurementLeadTime (     "Procurement Lead Time",
                                                           "Proc Ld Tm");
const LgFrScheduleDescriptor LGFRProductionReceiptVol      ( "Production Receipt Vol",
                                                             "Prod Re");
const LgFrScheduleDescriptor LGFRMRPManagedProductionPlusProcurementVolume
    							   ("Production Plus Procurement Vol",
    							    "Make + Buy Vol");

//        Things for the  "Planner-managed" (constrained) MultiPartSchedule
const LgFrScheduleDescriptor LGFRPlannerManagedSupplyVol (     "Planner Managed Supply Vol",
                                                               "Supply",
                                                               "",
                                                               "������خ�");
const LgFrScheduleDescriptor LGFRPlannerManagedRequiredVol (   "Planner Managed Explosion Required Vol",
                                                               "Expl Net Req",
                                                               "",
                                                               "MRP��ֳخ�");

const LgFrScheduleDescriptor LGFRPlannerManagedTotalPartDemandVol (   "Planner Managed Independent Demand",
                                                       "Indep Demand");

const LgFrScheduleDescriptor LGFRPlannerManagedConsVol ( "Planner Managed Explosion Part Consumption",
                                                          "Exp Consumed");

const LgFrScheduleDescriptor LGFRPlannerManagedTotalConsVol ( "Planner Managed Total Explosion Part Consumption",
                                                          "Total Exp Cons");

const LgFrScheduleDescriptor LGFRPlannerManagedCumSupplyVol (  "Planner Managed Cum Supply Vol",
                                                               "Cum Sup");
const LgFrScheduleDescriptor LGFRPlannerManagedCumRequiredVol ("Planner Managed Cum Explosion Required Vol",
                                                               "Cum Exl Net Req");
const LgFrScheduleDescriptor LGFRPlannerManagedExcessVol (     "Planner Managed Excess Vol",
                                                               "Excess",
                                                               "",
                                                               "ּޮ�خ�");
const LgFrScheduleDescriptor LGFRPlannerManagedFocussedShortageSchedule
					(     "Planner Managed Focussed Shortage Vol",
					      "Short Vol",
                                              "",
					      "̿�خ�");
const LgFrScheduleDescriptor LGFRPlannerManagedStockVol (     "Planner Managed Stock Vol",
                                                              "Stock",
                                                              "",
                                                              "�޲�خ�");
const LgFrScheduleDescriptor LGFRPlannerManagedScrapVol (     "Planner Managed Scrap Vol",
                                                              "Scrap");
const LgFrScheduleDescriptor LGFRPlannerManagedUsageVol (     "Planner Managed Usage Vol",
                                                              "Usage",
                                                              "",
                                                              "�ֳخ�");

//        Things for "Capacity" window 
const LgFrScheduleDescriptor LGFRCapacitySupplyVol (       "Capacity Supply Vol",
                                                           "Available",
                                                           "",
                                                           "ɳخ�");
const LgFrScheduleDescriptor LGFRCapacityRequiredVol (     "Capacity Required Vol",
                                                           "Expl Net Req",
                                                           "",
                                                           "MRP��ֳخ�");
const LgFrScheduleDescriptor LGFRCapacityExcessVol (       "Capacity Excess Vol",
                                                           "Excess",
                                                           "",
                                                           "�خ�");
const LgFrScheduleDescriptor LGFRCapacityUsageVol (       "Capacity Usage Vol",
                                                          "Usage",
                                                          "",
                                                          "�ֳخ�");
const LgFrScheduleDescriptor LGFRCapacityScrapVol (       "Capacity Scrap Vol",
                                                          "Scrap");
const LgFrScheduleDescriptor LGFRCapacityStockVol (       "Capacity Stock Vol",
                                                          "Stock");
const LgFrScheduleDescriptor LGFRCapacityUsagePct (       "Capacity Usage Percent",
                                                          "Usage%",
                                                          "",
                                                          "��޳��");

//        Things for  (demand) window 
const LgFrScheduleDescriptor LGFRCMRPDemandVol (       "CMRP Demand Volume",
                                                       "Demand Vol");
const LgFrScheduleDescriptor LGFRCMRPShipmentVol (       "CMRP Shipment Volume",
                                                         "Ship Sched");
const LgFrScheduleDescriptor LGFRCMRPBacklogVol (       "CMRP Backlog Volume",
                                                        "Backlog");

//        Things needed for netting forecast demands
const LgFrScheduleDescriptor LGFRCMRPCommittedDemandVol (       "Committed Demand Volume",
                                                                "CDV");
const LgFrScheduleDescriptor LGFRCMRPTotalCommittedDemandVol ( "Total Committed Demand Vol",
                                                               "TCDV");


//        Things for "Production" window
// const LgFrScheduleDescriptor LGFRProductionVolume = "Production Volume";  (same as default)

//        Things for "Procurement Budget" window (not being used for APICS demo)
const LgFrScheduleDescriptor LGFRBudgetAmount ( "Budget Amount",
                                                "Budget");

//        Things for "Procurement Summary" window
const LgFrScheduleDescriptor LGFRCMRPTotalMrpProcurementCost ( "Total MRP Procurement Cost",
                                                               "MRP Cost",
                                                               "",
                                                               "MRP �ݶ޸");
const LgFrScheduleDescriptor LGFRCMRPTotalWitProcurementCost ( "Total Implosion Procurement Cost",
                                                               "Implode Cost",
                                                               "",
                                                               "PRM �ݶ޸");
const LgFrScheduleDescriptor LGFRCMRPCumTotalMrpProcurementCost ( "Cum Total MRP Procurement Cost",
                                                                  "Cum MRP Cost",
                                                                  "",
                                                                  "MRP ٲ��");
const LgFrScheduleDescriptor LGFRCMRPCumTotalWitProcurementCost ( "Cum Total Implosion Procurement Cost",
                                                                  "Cum Impl. Cost",
                                                                  "",
                                                                  "PRM ٲ��");
const LgFrScheduleDescriptor LGFRCMRPDeltaCumTotalProcurementCost ( "Delta Cum Total Procurement Cost",
                                                                    "Delta MRP&Impl.",
                                                                    "",
                                                                    "ٲ���");
const LgFrScheduleDescriptor LGFRCostOfCapitalSavings ( "Cum Implosion Interest",
                                                        "Cum Interest",
                                                        "",
                                                        "ش� �ݻ�");

const LgFrScheduleDescriptor LGFRProductionVolumeTimesUnitCost ( "Production Volume Times Unit Cost",
                                                                 "Mfg Order Cost");
const LgFrScheduleDescriptor LGFRProcurementVolumeTimesUnitCost  ( "Procurement Volume Times Unit Cost",
                                                                   "Impl. P.O. Cost");
const LgFrScheduleDescriptor LGFRRequiredVolumeTimesUnitCost  ( "Required Volume Times Unit Cost",
                                                                "MRP Order Cost");

//        Things for "Demand Summary" information

const LgFrScheduleDescriptor 
  LGFRCMRPSumDemandVolToPart ( 
    "Total Demand Volume on Part",
    "Part Dem Vol"
    //             X  <-- X is last positin of Abbrev. Max len 15 characters    
  );

const LgFrScheduleDescriptor 
  LGFRCMRPCumSumDemandVolToPart ( 
    "Cumulative Total Demand Volume on Part",
    "CumPartDemVol"
    //             X  <-- X is last positin of Abbrev. Max len 15 characters    
  );

const LgFrScheduleDescriptor 
  LGFRCMRPSummaryOfCumDemandVol ( 
    "Summary of Cumulative Demand Volume",
    "TotCumDemVol"
    //             X  <-- X is last positin of Abbrev. Max len 15 characters    
  );

const LgFrScheduleDescriptor 
  LGFRCMRPCumSumDemandVolToPartTimesUnitCost ( 
    "Cumulative Total Demand Volume on Part Times Unit Cost",
    "$ CumPartDemVol"
    //             X  <-- X is last positin of Abbrev. Max len 15 characters    
  );

const LgFrScheduleDescriptor 
  LGFRCMRPSummaryOfCumDemandVolTimesUnitCost ( 
    "Summary of Cumulative Demand Volume Times Unit Cost",
    "$ TotCumDemVol"
    //             X  <-- X is last positin of Abbrev. Max len 15 characters    
  );


const LgFrScheduleDescriptor 
  LGFRCMRPSumShipmentVolToPart ( 
    "Total Shipment Volume of Part",
    "Part Ship Vol"
    //             X  <-- X is last positin of Abbrev. Max len 15 characters    
  );

const LgFrScheduleDescriptor 
  LGFRCMRPCumSumShipmentVolToPart ( 
    "Cumulative Total Shipment Volume of Part",
    "CumPartShipVol"
    //             X  <-- X is last positin of Abbrev. Max len 15 characters    
  );

const LgFrScheduleDescriptor 
  LGFRCMRPSummaryOfCumShipmentVol ( 
    "Summary of Cumulative Shipment Volume",
    "TotCumShipVol"
    //             X  <-- X is last positin of Abbrev. Max len 15 characters    
  );

const LgFrScheduleDescriptor 
  LGFRCMRPCumSumShipmentVolToPartTimesUnitCost ( 
    "Cumulative Total Shipment Volume of Part Times Unit Cost",
    "$CumPartShipVol"
    //             X  <-- X is last positin of Abbrev. Max len 15 characters    
  );

const LgFrScheduleDescriptor 
  LGFRCMRPSummaryOfCumShipmentVolTimesUnitCost ( 
    "Summary of Cumulative Shipment Volume Times Unit Cost",
    "$ TotCumShipVol"
    //             X  <-- X is last positin of Abbrev. Max len 15 characters    
  );



///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//                                                                   //
//               Following are new descriptors for ATP               //
//                                                                   //
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

// LGFRDefaultDemandVolume can be used to get data out of and into WIP but
// is not intended for end users.  Use one of the descriptors below for
// user-visible data
const LgFrScheduleDescriptor LGFRDefaultDemandVolume ( "Default Demand Volume",
                                                       "Req'd");

// -------------------------------------
// Things for the Committed demands window
// -------------------------------------
const LgFrScheduleDescriptor
      LGFRCommittedDemandVolume( "Committed Demand Volume",
                                 "Demand Vol");

const LgFrScheduleDescriptor
      LGFRCommittedCumDemandVolume ( "Committed Cum Demand Volume",
                                     "Cum Demand Vol");

const LgFrScheduleDescriptor
      LGFRCommittedShipmentVolume ( "Committed Shipment Volume",
                                    "Ship Sched");

const LgFrScheduleDescriptor
      LGFRCommittedCumShipmentVolume ( "Committed Cum Shipment Volume",
                                       "Cum Ship Sched");

const LgFrScheduleDescriptor
      LGFRCommittedBacklog ( "Committed Backlog",
                             "Backlog");

const LgFrScheduleDescriptor
      LGFRCommittedNonZeroBacklog ( "Committed Non-Zero Backlog",
				    "Backlog");

#if 0
// WIBNIS TO BE ADDED 
const LgFrScheduleDescriptor
      LGFRCommittedPriority = "Committed Priority";
const std::string
      LGFRCommittedPriorityAbbrev = "Priority";

const LgFrScheduleDescriptor
      LGFRCommittedRevenueTimesDemandVolume = "Revenue Times Demand Volume";
const std::string
      LGFRCommittedRevenueTimesDemandVolumeAbbrev = "Rev";

const LgFrScheduleDescriptor
      LGFRCommittedCumRevenueTimesDemandVolume = "Cum Revenue Times Demand Volume";
const std::string
      LGFRCommittedCumRevenueTimesDemandVolumeAbbrev = "Cum Rev";

const LgFrScheduleDescriptor
      LGFRCommittedRevenueTimesShipmentVolume = "Revenue Times Shipment Volume";
const std::string
      LGFRCommittedRevenueTimesShipmentVolumeAbbrev = "Ship Rev";

const LgFrScheduleDescriptor
      LGFRCommittedCumRevenueTimesShipmentVolume = 
          "Cum Revenue Times Shipment Volume";
const std::string
      LGFRCommittedCumRevenueTimesShipmentVolumeAbbrev = "Cum Ship Rev";
#endif

// -------- End of Committed demands window --------------

// -------------------------------------
// Things for the new opp demands window
// -------------------------------------

const LgFrScheduleDescriptor
      LGFRNewOpportunityDemandVolume ( "New Opportunity Demand Volume",
                                       "Desired");

const LgFrScheduleDescriptor
      LGFRNewOpportunityCumDemandVolume ( "New Opportunity Cum Demand Volume",
                                          "Cum Desired");

const LgFrScheduleDescriptor
      LGFRNewOpportunityShipmentVolume ( "New Opportunity Shipment Volume",
                                         "Available");

const LgFrScheduleDescriptor
      LGFRNewOpportunityCumShipmentVolume ( "New Opportunity Cum Shipment Volume",
                                            "Cum Available");

const LgFrScheduleDescriptor
      LGFRNewOpportunityBacklog ( "New Opportunity Backlog",
                                  "Backlog");

#if 0
// WIBNIS TO BE ADDED 
const LgFrScheduleDescriptor
      LGFRNewOpportunityPriority = "New Opportunity Priority";
const std::string
      LGFRNewOpportunityPriorityAbbrev = "Priority";
#endif

// -------- End of new opp demands window --------------

// -------------------------------------
// Things for the firm forecast demands window
// -------------------------------------

const LgFrScheduleDescriptor
      LGFRFirmForecastDemandVolume ( "Forecast Demand Volume",
                                     "Forecast Vol");

const LgFrScheduleDescriptor
      LGFRFirmForecastCumDemandVolume ( "Cum Forecast Demand Volume",
                                        "CumForecast Vol");

const LgFrScheduleDescriptor
      LGFRFirmForecastShipmentVolume ( "Available to Ship",
                                       "Avail to Ship");
const LgFrScheduleDescriptor
      LGFRFirmForecastCumShipmentVolume ( "Cum Available to Ship",
                                          "Cum ATS");
const LgFrScheduleDescriptor
      LGFRFirmForecastBacklog (
      "Forecast Shortage",
      "Shortage F/C"   
      //             X  <-- X is last positin of Abbrev. Max len 15 characters
      );

// -------- End of firm forecast demands window --------------

// ---------------------------------------------------------
// Things for computing the netted Firm Forecast Demand Vol
// ---------------------------------------------------------
const LgFrScheduleDescriptor
      LGFRTotalCommittedDemandVol( "Total Committed Demand Volume",
                                   "Total Dem. Vol");

// -------------------------------------
// Things for the material recovery plan window
// -------------------------------------

// LGFRMaterialSupplyVolume has a special implementation for this app.

const LgFrScheduleDescriptor
      LGFRMaterialCumSupplyVolume ( "Material Cum Supply Volume",
                                    "Cum Supply");

const LgFrScheduleDescriptor
      LGFRMaterialFocussedShortageSchedule ( "Material Focussed Shortage Schedule",
                                             "Shortage");

const LgFrScheduleDescriptor
      LGFRMaterialCumFocussedShortageSchedule ( 
         "Material Cum Focussed Shortage Schedule",
                                                "Cum Shortage");

// -------- End of material recovery plan window

// -------------------------------------
// Things for the capacity recovery plan window
// -------------------------------------

// LGFRCapacitySupplyVolume has a special implementation for this app.

const LgFrScheduleDescriptor
      LGFRCapacityFocussedShortageSchedule ( "Capacity Focussed Shortage Schedule",
                                             "Shortage");

const LgFrScheduleDescriptor
      LGFRCapacityCumFocussedShortageSchedule ( 
         "Capacity Cum Focussed Shortage Schedule",
                                                "Cum Shortage");
// -------- End of capacity recovery plan window


#ifdef USING_RISK_FORECAST
const LgFrScheduleDescriptor
      LGFRRiskForecastDemandVolume ( "Risk Forecast Demand Volume",
                                     "Max Req'd");
#endif
const LgFrScheduleDescriptor LGFRLastDemandVolume ( "Last Demand Volume",
                                                    "Last Req'd");


// Following are the descriptors for sort (for cmrp and atp)
const LgFrSortDescriptor
      LGFRSortByPart = "Sort By Part";
const LgFrSortDescriptor
      LGFRSortByPartNameAndDemand = "Sort By Part Name And Demand";
const LgFrSortDescriptor
      LGFRSortByCustomer = "Sort By Customer";
const LgFrSortDescriptor
      LGFRSortByOrderNumber = "Sort By Order Number";
const LgFrSortDescriptor
      LGFRSortByDemandQtyInPeriod = "Sort By Demand Qty In Period";
const LgFrSortDescriptor
      LGFRSortByCumDemandQtyInPeriod = "Sort By Cum Demand Qty In Period";
const LgFrSortDescriptor
      LGFRSortByCumDemandRevInPeriod = "Sort By Cum Demand Rev In Period";
const LgFrSortDescriptor
      LGFRSortByCumShipmentRevInPeriod = "Sort By Cum Shipment Rev In Period";
const LgFrSortDescriptor
      LGFRSortByPriority = "Sort By Priority";
const LgFrSortDescriptor
      LGFRSortByCriticality = "Sort By Criticality";

// Following are the descriptors for sort (for atp)
const LgFrSortDescriptor
      LGFRSortByAvailInPeriod = "Sort By Avail In Period";
const LgFrSortDescriptor
      LGFRSortByCumAvailInPeriod = "Sort By Cum Avail In Period";


// -------------------------------------
// Filter Descriptors
// -------------------------------------
const LgFrFilterDescriptor
      LGFRFilterNothing = "Filter Nothing";
const LgFrFilterDescriptor
      LGFRFilterOnDemandAndPartName = "Filter On Demand And Part Name";
const LgFrFilterDescriptor
      LGFRFilterOnPartName = "Filter On Part Name";
const LgFrFilterDescriptor
      LGFRFilterOnBacklogDemand = "Filter On Backlog Demand";
const LgFrFilterDescriptor
      LGFRFilterOnShortPart = "Filter On Short Part";
const LgFrFilterDescriptor
      LGFRFilterOnShortCapacity = "Filter On Short Capacity";

#endif
