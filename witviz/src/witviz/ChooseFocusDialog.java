/*
 * Created on Oct 22, 2004
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package witviz;

//import org.eclipse.jface.dialogs.*;


import java.util.Vector;

import org.eclipse.draw2d.graph.Node;
import org.eclipse.swt.SWT;
import org.eclipse.swt.events.SelectionAdapter;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.layout.FormAttachment;
import org.eclipse.swt.layout.FormData;
import org.eclipse.swt.layout.FormLayout;
import org.eclipse.swt.layout.RowLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Dialog;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.List;
import org.eclipse.swt.widgets.Listener;
import org.eclipse.swt.widgets.Shell;

/**
 * @author gresh
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
//public class ChooseFocusDialog extends Dialog {
public class ChooseFocusDialog extends Dialog {
    SimpleTree parentApp;
    final Shell shell;
    Label listLabel;
    List list;
    Shell parent;
    Composite c;

    Button PartsListButton, OperationsListButton, RawMaterialsListButton;
	Button CapacitiesListButton, DemandedPartsListButton;
	/**
	 * This is the default constructor
	 */
	public ChooseFocusDialog(Shell p, SimpleTree parent) {
		super(p);
        parentApp=parent;
	    shell= new Shell(p,SWT.DIALOG_TRIM| SWT.APPLICATION_MODAL | SWT.RESIZE);
	    //c = new Composite(shell,SWT.BORDER);
		// TODO Auto-generated constructor stub
		initializeShell1();
	}
	public void initializeShell1() {
		   parent = getParent();
		   setText("Choose Focus Object");
		   shell.setText("Choose Focus");

		   shell.addListener(SWT.Close, new Listener(){
		   public void handleEvent(Event event) {
		       event.doit=false;
		       close();
		       //your code here

		    }
		   });	   
		   
		   
	       //RowLayout layout = new RowLayout();
           FormLayout fl = new FormLayout();
	       // Optionally set layout fields.

	       //layout.wrap = true;

	       // Set the layout into the composite.

	        shell.setLayout(fl);
			Composite leftPanel = new Composite(shell,SWT.BORDER);
			RowLayout rl = new RowLayout();
			rl.type = SWT.VERTICAL;
			rl.fill = true;
			leftPanel.setLayout(rl);
			
			FormData fd33 = new FormData();
			fd33.left = new FormAttachment(0,5);
			fd33.top = new FormAttachment(0,5);
			fd33.bottom = new FormAttachment(100,-5);
			leftPanel.setLayoutData(fd33);
			
			Composite rightPanel = new Composite(shell,SWT.BORDER);
			rightPanel.setLayout(new FormLayout());
			FormData fd43 = new FormData();
			fd43.left = new FormAttachment(leftPanel,5);
			fd43.top = new FormAttachment(0,5);
			fd43.bottom = new FormAttachment(100,-5);
			fd43.right = new FormAttachment(100,-5);

			rightPanel.setLayoutData(fd43);
			
			
			listLabel = new Label(rightPanel,SWT.LEFT);
			listLabel.setText("");
			FormData fd3 = new FormData();
			fd3.left = new FormAttachment(0,5);
			fd3.top = new FormAttachment(0,5);
			fd3.right = new FormAttachment(100,-5);
			listLabel.setLayoutData(fd3);
			
			list = new List(rightPanel,SWT.SINGLE|SWT.V_SCROLL);
			FormData fd4 = new FormData();
			fd4.left = new FormAttachment(0,5);
			fd4.top = new FormAttachment(listLabel,5);
			fd4.bottom = new FormAttachment(100,-5);
			fd4.right = new FormAttachment(100,-5);
			list.setLayoutData(fd4);
			list.setSize(list.computeSize(200,400,false));
			list.addSelectionListener(
					   new SelectionAdapter()
					   {
					     public void widgetSelected(SelectionEvent e)
					     {
					       List list = (List) e.getSource();
					       String[] str = list.getSelection();
					       String focusName = str[0];
					       String focusType;
					       if (OperationsListButton.getSelection())
					       	   focusType = "operation";
					       else
					           focusType = parentApp.wo.getPartCategory(focusName);

						   String k = WitNode.createKey(focusType,focusName);
						   

						   if (parentApp.fulltree) {
							  Node n = (Node)parentApp.mainContents.hNodes.get(k);
							  parentApp.focusObject = (WitNode)n.data;
							  parentApp.mainContents.setSelected(parentApp.focusObject);
							  parentApp.mainContents.recenter(parentApp.focusObject);
							}
							//parentApp.detailContents.invalidate();
							//parentApp.shell.redraw();
							
							if (parentApp.fulltree) {
							    parentApp.focusChanged();
								parentApp.detailContents.FillInfoBox(focusType,focusName);
							    parentApp.detailContents.recenter(parentApp.focusObject);
							}
							else {
								WitNode wn = new WitNode();
								wn.setNodeType(focusType);
								wn.setText(focusName);
								parentApp.focusObject = wn;
								parentApp.focusChanged();
								parentApp.detailContents.FillInfoBox(focusType,focusName);

							}
					       
					      
					     }
					  }
					 );
			
			Label l = new Label(leftPanel,SWT.LEFT);
			l.setText("Focus of Model:");
			PartsListButton = new Button(leftPanel,SWT.RADIO);
			PartsListButton.setText("Choose From Parts");
			PartsListButton.addSelectionListener(new SelectionListener() {
				public void widgetSelected(SelectionEvent e) {
					listLabel.setText("parts");
					list.removeAll();
					
					Vector partslist = parentApp.getSortedPartsList();
					for (int i=0; i<partslist.size(); i++) {
						list.add((String)partslist.elementAt(i));
					}
					
					
					//list.redraw();
				}
				public void widgetDefaultSelected(SelectionEvent e) {
				//	graphList.setText(graphMethod);
				}
			});
			OperationsListButton = new Button(leftPanel,SWT.RADIO);
			OperationsListButton.setText("Choose From Operations");
			OperationsListButton.addSelectionListener(new SelectionListener() {
				public void widgetSelected(SelectionEvent e) {
					listLabel.setText("Operations");
					list.removeAll();
					
					
					Vector opslist = parentApp.getSortedOperationsList();
					for (int i=0; i<opslist.size(); i++) {
						list.add((String)opslist.elementAt(i));
					}
					

				}
				public void widgetDefaultSelected(SelectionEvent e) {
				//	graphList.setText(graphMethod);
				}
			});
			RawMaterialsListButton = new Button(leftPanel,SWT.RADIO);
			RawMaterialsListButton.setText("Choose From Non-product Parts");
			RawMaterialsListButton.addSelectionListener(new SelectionListener() {
				public void widgetSelected(SelectionEvent e) {
					listLabel.setText("Non-product parts");
					list.removeAll();
					String[] rmlist = parentApp.getSortedRawMaterialsList();
					for (int i=0; i<rmlist.length; i++) {
						list.add(rmlist[i]);
					}

				}
				public void widgetDefaultSelected(SelectionEvent e) {
				//	graphList.setText(graphMethod);
				}
			});
			CapacitiesListButton = new Button(leftPanel,SWT.RADIO);
			CapacitiesListButton.setText("Choose From Capacities");
			CapacitiesListButton.addSelectionListener(new SelectionListener() {
				public void widgetSelected(SelectionEvent e) {
					listLabel.setText("capacities");
					list.removeAll();
					Vector partslist = parentApp.getSortedCapacitiesList();
					for (int i=0; i<partslist.size(); i++) {
						list.add((String)partslist.elementAt(i));
					}
					//list.redraw();
				}
				public void widgetDefaultSelected(SelectionEvent e) {
				//	graphList.setText(graphMethod);
				}
			});
			DemandedPartsListButton = new Button(leftPanel,SWT.RADIO);
			DemandedPartsListButton.setText("Choose From Demanded Parts");
			DemandedPartsListButton.addSelectionListener(new SelectionListener() {
				public void widgetSelected(SelectionEvent e) {
					listLabel.setText("demanded parts");
					list.removeAll();
					Vector partslist = parentApp.getSortedDemandedPartsList();
					for (int i=0; i<partslist.size(); i++) {
						list.add((String)partslist.elementAt(i));
					}
					//list.redraw();
				}
				public void widgetDefaultSelected(SelectionEvent e) {
				//	graphList.setText(graphMethod);
				}
			});
			
			
			PartsListButton.setSelection(true);
	        shell.pack();

	}
	
	public void open () {
		shell.setVisible(true);
		PartsListButton.setSelection(true);
		OperationsListButton.setSelection(false);
		RawMaterialsListButton.setSelection(false);
	    CapacitiesListButton.setSelection(false);
	    DemandedPartsListButton.setSelection(false);

		listLabel.setText("parts");
		list.removeAll();
		Vector partslist = parentApp.getSortedPartsList();
		for (int i=0; i<partslist.size(); i++) {
			list.add((String)partslist.elementAt(i));
		}
		Display display = parent.getDisplay();
		while (!shell.isDisposed()) {
			if (!display.readAndDispatch()) display.sleep();
		}

	}	
	public void close() {
		//just make invisible
		
		shell.setVisible(false);
	}
	
	//public boolean close()	{		// copy widgets' input to private members		
			
	//	return super.close();	
	//}	
	
	 // default (English language, United States)

}








