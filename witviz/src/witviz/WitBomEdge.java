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
public class WitBomEdge extends WitEdge {
    private WitNode operation;
	private WitNode part;
	private int bomEntry;
	public WitBomEdge() {
		super();
		edgeType = "bom";

	}
    public String createKey() {
		String key;
		String op = operation.getText();
		String pa = part.getText();
		key = createKey(op,pa,bomEntry);
		text = key;
        return key;
	}
	public static String createKey(String op, String part, int entry) {
		String key;
		key = "BOM: Operation: "+op+ " Part: " + part + " BomEntry: "+ entry;
        return key;
		
	}
    public WitNode getOperation() {
		return operation;
    }
	public String getSource() {
		return operation.getText();
	}
	public String getTarget() {
		return part.getText();
	}
	public void setOperation(WitNode o) {
		operation = o;
		source = operation.getText();
	}
	public WitNode getPart() {
		return part;	
	}
	public void setPart(WitNode p) {
		part=p;
		target = part.getText();
	}
	public void setBomEntry(int b) {
		bomEntry = b;
	}
	public int getBomEntry() {
		return bomEntry;
	}
	

}
