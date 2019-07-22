/*
 * Created on Jan 7, 2005
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package witviz;



import java.util.Hashtable;

import org.eclipse.draw2d.ColorConstants;
import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.StyleRange;
import org.eclipse.swt.custom.StyledText;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.layout.RowLayout;
import org.eclipse.swt.widgets.Button;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Dialog;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.Listener;
import org.eclipse.swt.widgets.Shell;

/**
 * @author gresh
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
abstract public class OptionsDialog extends Dialog {
    Hashtable buttonTable=new Hashtable();
    WitObject wo;
    final Shell shell;
    final Composite buttonC;
    Button nonDefault, all, none;
    SimpleTree parentApp;
    org.eclipse.swt.graphics.Color HighlightColor = ColorConstants.yellow;
    org.eclipse.swt.graphics.Color InvalidColor = ColorConstants.lightGray;
    public enum HIGHLIGHT {
        DEFAULT, HIGHLIGHT, INVALID
    }
    Shell parent;

	/**
	 * @param parent
	 */
	public OptionsDialog(Shell parent, SimpleTree p, String obj) {
		super(parent);
		parentApp=p;
	    shell= new Shell(parent, SWT.DIALOG_TRIM | SWT.APPLICATION_MODAL );
	    buttonC = new Composite(shell,SWT.BORDER);
		shell.setText(obj+" attributes");

		// TODO Auto-generated constructor stub
		initializeShell();
	}

	/**
	 * @param parent
	 * @param style
	 */
	public OptionsDialog(Shell parent, int style) {
		super(parent, style);
	    shell= new Shell(parent, SWT.DIALOG_TRIM | SWT.APPLICATION_MODAL );
	    buttonC = new Composite(shell,SWT.BORDER);
		initializeShell();
		// TODO Auto-generated constructor stub
	}
    public void setWitObject(WitObject witobject) {
		wo=witobject;    	
    }
	public void initializeShell() {

		parent = getParent();
		//shell.setText(getText());
		shell.setSize(300,400);
	
		
		
		shell.addListener(SWT.Close, new Listener(){
			   public void handleEvent(Event event) {
			       event.doit=false;
			       close();
			       //your code here

			    }
			   });	   
	    GridLayout gridLayout1 = new GridLayout();
	    gridLayout1.numColumns = 1;
	    shell.setLayout(gridLayout1);
		
	    GridLayout gridLayout = new GridLayout();
	    gridLayout.numColumns = 3;
	    buttonC.setLayout(gridLayout);
	    
	    Composite selections = new Composite(shell,0);
	    selections.setLayout(new RowLayout());
	    try {
			wo = new WitObject();
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	    fillShell(); //needs to be done by the subclass
	    wo.finalize();
	    all = new Button(selections, SWT.PUSH);
		all.setText ("Select All");
		Listener allListener =new Listener () {
			public void handleEvent (Event event) {
				//result [0] = event.widget == ok;
				selectAll();
			}
		};
		all.addListener (SWT.Selection, allListener);
		selectAll();
		
	    none = new Button(selections, SWT.PUSH);
		none.setText ("Select None");
		Listener noneListener =new Listener () {
			public void handleEvent (Event event) {
				//result [0] = event.widget == ok;
				selectNone();
			}
		};
		none.addListener (SWT.Selection, noneListener);
		


        Composite composite = new Composite(shell,0);
		composite.setLayout (new RowLayout ());

		final Button ok = new Button (composite, SWT.PUSH);
		ok.setText ("Ok");
		Button cancel = new Button (composite, SWT.PUSH);
		cancel.setText ("Cancel");
		Listener listener =new Listener () {
			public void handleEvent (Event event) {
				//result [0] = event.widget == ok;
				shell.setVisible (false);
				
			}
		};
		ok.addListener (SWT.Selection, listener);
		cancel.addListener (SWT.Selection, listener);
		shell.pack ();
		shell.setVisible(false);
	}
	public void open () {
		shell.setVisible(true);

		Display display = parent.getDisplay();
		while (!shell.isDisposed()) {
			if (!display.readAndDispatch()) display.sleep();
		}

	}
	public void selectAll() {
		//get all the buttons in the shell
		for (int i=0; i<buttonC.getChildren().length; i++) {
			Button b = (Button)buttonC.getChildren()[i];
			b.setSelection(true);
		}
	}
	public void selectNone() {
		//get all the buttons in the shell
		for (int i=0; i<buttonC.getChildren().length; i++) {
			Button b = (Button)buttonC.getChildren()[i];
			b.setSelection(false);
		}
	}
	protected void AppendText(StyledText textBox,  HIGHLIGHT hLight, String string) {
		   boolean showOnlyNonDefault = parentApp.showOnlyNonDefault;
		   boolean doHighlight=parentApp.highlightNonDefault;
		   if (showOnlyNonDefault) {	
			   if (hLight==HIGHLIGHT.HIGHLIGHT) {
				   //append it
				   textBox.append(string);
			   }
		   }
		   else {
			   //append it
			   //highlight it if appropriate
			   if (doHighlight&&hLight==HIGHLIGHT.HIGHLIGHT) {
				    textBox.append(string);
				    String[] splitString = string.split("\\n");
				    int numLinesInString = splitString.length;
				    int linecounter = textBox.getLineCount()-1-numLinesInString;
		   	    	textBox.setLineBackground(linecounter,numLinesInString,HighlightColor);
			   }
			   else if (hLight==HIGHLIGHT.DEFAULT) {
				   textBox.append(string);
			   }
			   else if (hLight==HIGHLIGHT.INVALID){
				    StyleRange styleRange = new StyleRange();
					styleRange.start = textBox.getCharCount();
					styleRange.length = string.length();
					//styleRange.fontStyle = SWT.BOLD;
					Display display = shell.getDisplay();
					styleRange.foreground = display.getSystemColor(SWT.COLOR_GRAY);
					textBox.append(string);					
					textBox.setStyleRange(styleRange);
			   }
		   }
	}
	public boolean optionDesired(String optionName) {
		Button b = (Button)buttonTable.get(optionName);
		if (b.isEnabled())
		   return b.getSelection();
		else
			return false;
	}
	protected boolean CheckVector(float[][] vec, int numperiods, float checkValue) {
		for (int i=0; i<numperiods; i++) {
			if (vec[0][i]!= checkValue) return true;
		}
		return false;
	}
	protected boolean CheckVector(int[][] vec, int numperiods, int checkValue) {
		for (int i=0; i<numperiods; i++) {
			if (vec[0][i]!= checkValue) return true;
		}
		return false;
	}
	protected boolean CheckVector(boolean[][] vec, int numperiods, boolean checkValue) {
		for (int i=0; i<numperiods; i++) {
			if (vec[0][i]!=checkValue)	return true;
		}
		return false;
	}
abstract public void fillShell();
abstract public void getOptionsString(String partName, StyledText textBox);
abstract public void setEnabled();
public void addOptionButton(String name)	{
	    Composite parent = buttonC;
	    Button b = new Button(parent,SWT.CHECK);
	    b.setText(name);
	    buttonTable.put(name,b);
	    Button b1 = (Button)buttonTable.get(name);
		b.addSelectionListener(new SelectionListener() {
			public void widgetSelected(SelectionEvent e) {
				//int foo=1;
				//Button b = (Button)e.widget;
				//boolean isSelected = b.getSelection();
				//String text  = b.getText();
			}
		    public void widgetDefaultSelected(SelectionEvent e) {
			    int foo=1;
			}
		});

}
public void close() {
	//just make invisible
	shell.setVisible(false);
}
}
