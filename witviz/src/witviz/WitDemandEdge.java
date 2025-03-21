/*
 * Created on Nov 24, 2004
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package witviz;


/**
 * @author gresh
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class WitDemandEdge extends WitEdge {
	private WitDemandNode demand;
	private WitNode part;
	
	public WitDemandEdge() {
		super();
	}
	public void setPart(WitNode p) {
		part = p;
	}
	public WitNode getPart() {
		return part;
	}
	public void setDemand(WitDemandNode d) {
		demand = d;
	}
	public WitDemandNode getDemand() {
		return demand;
	}
	public String getSource() {
		return demand.getText();
	}
	public String getTarget() {
		return part.getText();
	}
	public String createKey() {
		String key;
		String demandname, partname;
		demandname = demand.getDemandName();
		partname = part.getText();
		key = createKey(demandname, partname);
		return key;

	}
	public static String createKey(String demandname, String partname) {
		String key;
		key = "DemandEdge: " + " Demand: " + demandname + " Part: " + partname;
	    return key;	
	}
	
	
	
	
	
	
	

}
