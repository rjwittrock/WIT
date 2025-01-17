/*
 * Created on Nov 4, 2004n
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package witviz;

// import org.eclipse.swt.program.*;

import java.io.File;
import java.io.FileInputStream;
import java.lang.reflect.Method;
import java.util.Arrays;
import java.util.Iterator;
import java.util.Set;
import java.util.TreeSet;
import java.util.Vector;

import org.eclipse.draw2d.ChopboxAnchor;
import org.eclipse.draw2d.ColorConstants;
import org.eclipse.draw2d.Figure;
import org.eclipse.draw2d.FigureCanvas;
import org.eclipse.draw2d.IFigure;
import org.eclipse.draw2d.geometry.Point;
import org.eclipse.draw2d.graph.DirectedGraph;
import org.eclipse.gef.examples.pde.AbstractGraphDemo;
import org.eclipse.swt.SWT;
import org.eclipse.swt.custom.CTabFolder;
import org.eclipse.swt.custom.CTabFolder2Listener;
import org.eclipse.swt.custom.CTabFolderEvent;
import org.eclipse.swt.custom.CTabItem;
import org.eclipse.swt.custom.SashForm;
import org.eclipse.swt.custom.StyledText;
import org.eclipse.swt.events.DisposeEvent;
import org.eclipse.swt.events.DisposeListener;
import org.eclipse.swt.events.SelectionEvent;
import org.eclipse.swt.events.SelectionListener;
import org.eclipse.swt.graphics.Cursor;
import org.eclipse.swt.graphics.Font;
import org.eclipse.swt.graphics.GC;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.graphics.ImageData;
import org.eclipse.swt.layout.FormAttachment;
import org.eclipse.swt.layout.FormData;
import org.eclipse.swt.layout.FormLayout;
import org.eclipse.swt.layout.GridData;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.layout.RowLayout;
import org.eclipse.swt.printing.PrintDialog;
import org.eclipse.swt.printing.Printer;
import org.eclipse.swt.printing.PrinterData;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Event;
import org.eclipse.swt.widgets.FileDialog;
import org.eclipse.swt.widgets.Listener;
import org.eclipse.swt.widgets.Menu;
import org.eclipse.swt.widgets.MenuItem;
import org.eclipse.swt.widgets.Shell;
import org.eclipse.swt.widgets.ToolBar;
import org.eclipse.swt.widgets.ToolItem;

import witviz.WitObject.STATUS;






//import com.ibm.cbo.graphPanel.chartbase.SVG;

// import org.eclipse.draw2d.examples.graph.AbstractGraphDemo.TopOrBottomAnchor;

public class SimpleTree extends AbstractGraphDemo {
	static Shell shell;

	FigureCanvas figureCanvas = null;

	IFigure contents;

	static WitObject wo = null;
	String WitDataDir=null;
	String witFileName=null;

	boolean doOpalPicture = true;

	boolean hasOpalData = false;

	//static WitObject wodetail = null;

	static StyledText TextBox1;

	static StyledText TextBox2;

	CreateTreeFigure mainContents;

	CreateTreeFigure detailContents;

	Composite splitPaneComposite;

	static SashForm sash_form;

	static Composite rightPanel, leftPanel;

	static CTabFolder leftTab, rightTab;

	static SashForm rightSash, leftSash;

	private FigureCanvas dc;

	boolean outputData = false;
	boolean detailLast = false;

	// static Button refocusButton;
	// static Button expandButton;

	// static Button displayGlobalButton;
	static FileDialog fileDialog;

	static ChooseFocusDialog chooseFocusDialog;

	static BomOptionsDialog bomOptionDialog;

	static BopOptionsDialog bopOptionDialog;

	static PartOptionsDialog partOptionDialog;

	static DemandEdgeOptionsDialog demandEdgeOptionDialog;

	static GlobalOptionsDialog globalOptionDialog;

	static OperationOptionsDialog operationOptionDialog;

	static SubstituteOptionsDialog substituteOptionDialog;

	static GlobalDataDialog globalDataDialog;
	
	static PreferencesDialog preferencesDialog;
	
	static ChooseScenarioDialog scenarioChooser;
	
	static ChooseObjectiveDialog objectiveChooser;
	
	//static ObjectiveDialog objectiveChooser;

	boolean fulltree;


	// String focusName=null;
	// String focusType=null;
	Object focusObject = null;

	String appName;

	Cursor waitCursor;

	Composite OpalComposite;

	boolean highlightNonDefault;
	boolean showOnlyNonDefault;

	// private static int port = com.ibm.opal.server.OpalServer.DEFAULTPORT;
	// private static DataSource opaldatasource;
	// private static String host = "localhost";
	// OpalServer opalServer;
	// OpalPictureDialog OpalPicture;
	int leftMargin, rightMargin, topMargin, bottomMargin;

	String tabs;

	GC gc;

	Display d;

	Font font, printerFont;

	int tabWidth = 0;

	MenuItem focus, openFull, openPartial, print, exit, part, operation,
			demand, substitute, global, BOM, BOP;

	MenuItem highlight, shownondefault, showglobaldata;//, statistics;

	MenuItem postprocess, heurimplosion, optimplosion,stochimplosion,mrp,setScenario,setObjective;

	public static void main(String[] args) {
		
		new SimpleTree().run(args);
	}

	/**
	 * @see org.eclipse.draw2d.examples.AbstractExample#getContents()
	 */





	protected IFigure getContents() {
		getFigureCanvas().setBackground(ColorConstants.white);
		// getFigureCanvas().setVerticalScrollBarVisibility(FigureCanvas.ALWAYS);
		// getFigureCanvas().setHorizontalScrollBarVisibility(FigureCanvas.ALWAYS);
		mainContents = new CreateTreeFigure(wo, true, this, getFigureCanvas(),
				80, 40);
		return mainContents;

	}

	protected IFigure getDetailContents() {
		getDetailCanvas().setBackground(ColorConstants.white);
		// getDetailCanvas().setVerticalScrollBarVisibility(FigureCanvas.ALWAYS);
		// getDetailCanvas().setHorizontalScrollBarVisibility(FigureCanvas.ALWAYS);
		detailContents = new CreateTreeFigure(wo, focusObject, this,
				getDetailCanvas(), 80, 40);
		return detailContents;

	}

	protected FigureCanvas getDetailCanvas() {
		return dc;
	}

	public void focusChanged() {
		// mainContents.SelectedType = focusType;
		// mainContents.SelectedName = focusName;
		mainContents.SelectedObject = focusObject;
		detailContents = (CreateTreeFigure) getDetailContents();
		detailContents.setTextBox(TextBox2);
		getDetailCanvas().setContents(detailContents);

		if (fulltree) {
			if (focusObject.getClass().toString()
					.equals("class witviz.WitNode")
					|| focusObject.getClass().toString().equals(
							"class witviz.WitDemandNode")) {

				detailContents.ColorSelf(mainContents.SelectedObject);
				// detailContents.SelectedType = mainContents.SelectedType;
				// detailContents.SelectedName = mainContents.SelectedName;
			}
		}

	}

	protected void setDetailCanvas(FigureCanvas canvas) {
		this.dc = canvas;
	}

	protected void run(String[] args) {
		d = Display.getDefault();
		shell = new Shell(d);
		
		String argfilepath=null;
		String envfilepath = System.getenv("WITVIZDATAPATH");
		//get some info about it
		if (envfilepath !=null) {
			File directory = new File(envfilepath);
		    if ( ! directory.exists() ) {
		        System.out.println("Specified environment variable WITVIZDATAPATH data path does not exist.");
		        envfilepath = null;
		    }
		    
		}

		try {
			if (args.length==1) 
				argfilepath = new File(args[0]).getCanonicalPath();
		}
		catch (Exception e) {
			e.printStackTrace();
		}
		
		
		if (argfilepath !=null) {
			File directory = new File(argfilepath);
			if ( ! directory.exists() ) {
			     System.out.println("Specified command line wit data path does not exist.");
			     argfilepath = null;
			}
		}
		
		String overallfilepath = null;
		if (argfilepath==null) 
			overallfilepath = envfilepath;
		else
			overallfilepath = argfilepath;
		
		//check whether it is a directory or a file
		if (overallfilepath !=null) {
			File directory = new File(overallfilepath);
			if (directory.isDirectory() )
			       WitDataDir = overallfilepath;
			else {
				witFileName = overallfilepath;
			}
		}
		
		Image WitImage = new Image(null, "images/witmodel.gif");
		shell.setImage(WitImage);
		waitCursor = new Cursor(d, SWT.CURSOR_WAIT);

		globalDataDialog = new GlobalDataDialog(shell, this);

		// Base.Socketting(0);
		// try {
		// opalServer = new OpalServer();
		// }
		// catch (Exception e) {
		// e.printStackTrace();
		// }
		// opaldatasource = new DataSource(host, port+1);

		appName = getClass().getName();
		appName = appName.substring(appName.lastIndexOf('.') + 1);
		shell.setText("WitViz");
		shell.setLayout(new GridLayout());

		Menu bar = new Menu(shell, SWT.BAR);
		shell.setMenuBar(bar);

		ToolBar toolbar = new ToolBar(shell, SWT.HORIZONTAL | SWT.FLAT);
		GridData gridData = new GridData();
		gridData.horizontalAlignment = GridData.FILL;
		gridData.grabExcessHorizontalSpace = true;
		toolbar.setLayoutData(gridData);

		// toolbar.setLayoutData(BorderLayout.NORTH);

		final ToolItem openFullItem = new ToolItem(toolbar, SWT.PUSH);
		Image fullOpenIcon = new Image(null, "images/newfile_wiz.gif");
		openFullItem.setImage(fullOpenIcon);
		// openFullItem.setText("Open Full");
		openFullItem.setToolTipText("Open Full");
		openFullItem.addSelectionListener(new SelectionListener() {
			public void widgetSelected(SelectionEvent event) {
				OpenFullModel();
			}

			public void widgetDefaultSelected(SelectionEvent event) {

			}
		});

		final ToolItem openPartialItem = new ToolItem(toolbar, SWT.PUSH);
		Image fullPartialIcon = new Image(null, "images/newpartialfile_wiz.gif");
		openPartialItem.setImage(fullPartialIcon);
		// openPartialItem.setText("Open Partial");
		openPartialItem.setToolTipText("Open Partial");
		openPartialItem.addSelectionListener(new SelectionListener() {
			public void widgetSelected(SelectionEvent event) {
				OpenPartialModel();
			}

			public void widgetDefaultSelected(SelectionEvent event) {

			}
		});

		final ToolItem printItem = new ToolItem(toolbar, SWT.PUSH);
		Image PrintIcon = new Image(null, "images/printview_tsk.gif");
		printItem.setImage(PrintIcon);
		// printItem.setText("Print");
		printItem.setToolTipText("Print");
		printItem.addSelectionListener(new SelectionListener() {
			public void widgetSelected(SelectionEvent event) {
				Print();
			}

			public void widgetDefaultSelected(SelectionEvent event) {

			}
		});

		MenuItem fileItem = new MenuItem(bar, SWT.CASCADE);
		fileItem.setText("File");
		Menu submenuFile = new Menu(shell, SWT.DROP_DOWN);
		fileItem.setMenu(submenuFile);
		openFull = new MenuItem(submenuFile, SWT.PUSH);
		openFull.addListener(SWT.Selection, new Listener() {
			public void handleEvent(Event e) {
				OpenFullModel();

			}
		});
		openFull.setText("Open Full Model");

		openPartial = new MenuItem(submenuFile, SWT.PUSH);
		openPartial.addListener(SWT.Selection, new Listener() {
			public void handleEvent(Event e) {
				OpenPartialModel();
			}
		});
		openPartial.setText("Open Partial Model");

		MenuItem print = new MenuItem(submenuFile, SWT.PUSH);
		print.addListener(SWT.Selection, new Listener() {
			public void handleEvent(Event e) {
				Print();
			}
		});
		print.setText("Print");
		// openFull.setText ("Select &All\tCtrl+A");
		// item.setAccelerator (SWT.CTRL + 'A');
		
		exit = new MenuItem(submenuFile, SWT.PUSH);
		exit.setText("Exit");
		exit.addListener(SWT.Selection, new Listener() {
			public void handleEvent(Event e) {
				shell.dispose();
			}
		});

		MenuItem editItem = new MenuItem(bar, SWT.CASCADE);
		editItem.setText("Edit");
		Menu submenuEdit = new Menu(shell, SWT.DROP_DOWN);
		editItem.setMenu(submenuEdit);

		focus = new MenuItem(submenuEdit, SWT.PUSH);
		focus.addListener(SWT.Selection, new Listener() {
			public void handleEvent(Event e) {
				ChooseFocus();
			}
		});
		focus.setText("Choose Focus");
		focus.setEnabled(false);

		MenuItem options = new MenuItem(submenuEdit, SWT.CASCADE);
		options.setText("Options");
		Menu optionsDropdown = new Menu(shell, SWT.DROP_DOWN);
		options.setMenu(optionsDropdown);

		part = new MenuItem(optionsDropdown, SWT.PUSH);
		part.addListener(SWT.Selection, new Listener() {
			public void handleEvent(Event e) {
				partOptionDialog.open();
			}
		});
		part.setText("Part");

		operation = new MenuItem(optionsDropdown, SWT.PUSH);
		operation.addListener(SWT.Selection, new Listener() {
			public void handleEvent(Event e) {
				operationOptionDialog.open();
			}
		});
		operation.setText("Operation");

		BOM = new MenuItem(optionsDropdown, SWT.PUSH);
		BOM.addListener(SWT.Selection, new Listener() {
			public void handleEvent(Event e) {
				bomOptionDialog.open();
			}
		});
		BOM.setText("BOM");

		BOP = new MenuItem(optionsDropdown, SWT.PUSH);
		BOP.addListener(SWT.Selection, new Listener() {
			public void handleEvent(Event e) {
				bopOptionDialog.open();
			}
		});
		BOP.setText("BOP");

		demand = new MenuItem(optionsDropdown, SWT.PUSH);
		demand.addListener(SWT.Selection, new Listener() {
			public void handleEvent(Event e) {
				demandEdgeOptionDialog.open();
			}
		});
		demand.setText("Demand");

		substitute = new MenuItem(optionsDropdown, SWT.PUSH);
		substitute.addListener(SWT.Selection, new Listener() {
			public void handleEvent(Event e) {
				substituteOptionDialog.open();
			}
		});
		substitute.setText("Substitute");

		global = new MenuItem(optionsDropdown, SWT.PUSH);
		global.addListener(SWT.Selection, new Listener() {
			public void handleEvent(Event e) {
				globalOptionDialog.open();
			}
		});
		global.setText("Global");
		

		MenuItem prefs = new MenuItem(submenuEdit, SWT.PUSH);
		prefs.setText("Preferences");
		prefs.addListener(SWT.Selection, new Listener() {
			public void handleEvent(Event e) {
				preferencesDialog.open();
			}
		});



		MenuItem viewItem = new MenuItem(bar, SWT.CASCADE);
		viewItem.setText("View");
		Menu submenuView = new Menu(shell, SWT.DROP_DOWN);
		viewItem.setMenu(submenuView);

		showglobaldata = new MenuItem(submenuView, SWT.PUSH);
		showglobaldata.addListener(SWT.Selection, new Listener() {
			public void handleEvent(Event e) {
				ShowGlobal();
			}
		});
		showglobaldata.setText("Global Data");
		showglobaldata.setEnabled(false);

		//statistics = new MenuItem(submenuView, SWT.PUSH);
		//statistics.setEnabled(false);
		//statistics.addListener(SWT.Selection, new Listener() {
		//	public void handleEvent(Event e) {
		//		 if (!hasOpalData) {
		//		 PopulateOpalDataSource(wo,witFileName);
		//		 }
		//		 hasOpalData=true;
		//		 OpalPicture.open();
		//	}
		//});
		//statistics.setText("Statistics");

		MenuItem modelItem = new MenuItem(bar, SWT.CASCADE);
		modelItem.setText("Actions");
		Menu submenuModel = new Menu(shell, SWT.DROP_DOWN);
		modelItem.setMenu(submenuModel);
		
		heurimplosion = new MenuItem(submenuModel, SWT.PUSH);
		heurimplosion.addListener(SWT.Selection, new Listener() {
			public void handleEvent(Event e) {
				try {
				HeuristicImplosion();
				}
				catch (Exception ex) {
					ex.printStackTrace();
				}
			}
		});
		heurimplosion.setText("Heuristic Implosion");
		heurimplosion.setEnabled(false);
		
		optimplosion = new MenuItem(submenuModel, SWT.PUSH);
		optimplosion.addListener(SWT.Selection, new Listener() {
			public void handleEvent(Event e) {
				try {
				OptimizingImplosion();
				}
				catch (Exception ex) {
					ex.printStackTrace();
				}
			}
		});
		optimplosion.setText("Optimizing Implosion");
		optimplosion.setEnabled(false);
		
		stochimplosion = new MenuItem(submenuModel, SWT.PUSH);
		stochimplosion.addListener(SWT.Selection, new Listener() {
			public void handleEvent(Event e) {
				try{
				StochasticImplosion();
				}
				catch (Exception ex) {
					ex.printStackTrace();
				}
				
			}
		});
		stochimplosion.setText("Stochastic Implosion");
		stochimplosion.setEnabled(false);
		
		mrp = new MenuItem(submenuModel, SWT.PUSH);
		mrp.addListener(SWT.Selection, new Listener() {
			public void handleEvent(Event e) {
				try {
				MRP();
				}
				catch (Exception ex) {
					ex.printStackTrace();
				}
			}
		});
		mrp.setText("MRP");
		mrp.setEnabled(false);
		
		
		postprocess = new MenuItem(submenuModel, SWT.PUSH);
		postprocess.addListener(SWT.Selection, new Listener() {
			public void handleEvent(Event e) {
				try {
				Postprocess();
				}
				catch (Exception ex) {
					ex.printStackTrace();
				}
			}
		});
		postprocess.setText("Post process model");
		postprocess.setEnabled(false);
		
		
		MenuItem set = new MenuItem(bar, SWT.CASCADE);
		set.setText("Set");
		Menu submenuSet = new Menu(shell, SWT.DROP_DOWN);
		set.setMenu(submenuSet);
		setScenario = new MenuItem(submenuSet, SWT.PUSH);
		setScenario.addListener(SWT.Selection, new Listener() {
			public void handleEvent(Event e) {
				setScenario();
			}
		});
		setScenario.setText("Set Current Scenario");
		setScenario.setEnabled(false); 
		
		setObjective = new MenuItem(submenuSet, SWT.PUSH);
		setObjective.addListener(SWT.Selection, new Listener() {
			public void handleEvent(Event e) {
				setObjective();
			}
		});
		setObjective.setText("Set Current Objective");
		setObjective.setEnabled(false); 


		// OpalPicture = new OpalPictureDialog(shell,this);

		splitPaneComposite = new Composite(shell, 0);
		gridData = new GridData();
		gridData.horizontalAlignment = GridData.FILL;
		gridData.verticalAlignment = GridData.FILL;
		gridData.grabExcessHorizontalSpace = true;
		gridData.grabExcessVerticalSpace = true;
		splitPaneComposite.setLayoutData(gridData);

		// splitPaneComposite.setLayoutData(BorderLayout.CENTER);
		splitPaneComposite.setLayout(new FormLayout());

		sash_form = new SashForm(splitPaneComposite, SWT.HORIZONTAL);
		FormData fd3 = new FormData(); // fill the area
		fd3.left = new FormAttachment(0, 5);
		fd3.top = new FormAttachment(0, 5);
		fd3.bottom = new FormAttachment(100, -5);
		fd3.right = new FormAttachment(100, -5);
		sash_form.setLayoutData(fd3);

		leftTab = new CTabFolder(sash_form, SWT.TOP);
		leftTab.setSimple(false);
		leftTab.setMaximizeVisible(true);
		leftTab.setMinimizeVisible(true);

		leftTab.addCTabFolder2Listener(new CTabFolder2Listener() {
			public void showList(CTabFolderEvent event) {

			}

			public void maximize(CTabFolderEvent event) {
				sash_form.setMaximizedControl(leftTab);
			}

			public void minimize(CTabFolderEvent event) {
				sash_form.setMaximizedControl(rightTab);
			}

			public void close(CTabFolderEvent event) {

			}

			public void restore(CTabFolderEvent event) {
				sash_form.setMaximizedControl(null);
			}
		});
		CTabItem leftTabItem = new CTabItem(leftTab, SWT.NONE);
		leftTabItem.setText("Full Model");
		leftTab.setSelection(0);

		ToolBar toolbarFull = new ToolBar(leftTab, SWT.HORIZONTAL | SWT.FLAT);

		final ToolItem restoreFull = new ToolItem(toolbarFull, SWT.PUSH);
		Image restoreIcon = new Image(null, "images/restore.gif");
		restoreFull.setImage(restoreIcon);
		// restoreFull.setText("Restore");
		restoreFull.setToolTipText("Restore");
		restoreFull.addSelectionListener(new SelectionListener() {
			public void widgetSelected(SelectionEvent event) {
				sash_form.setMaximizedControl(null);
			}

			public void widgetDefaultSelected(SelectionEvent event) {

			}
		});
		leftTab.setTabHeight(Math.max(toolbarFull.computeSize(SWT.DEFAULT,
				SWT.DEFAULT).y, leftTab.getTabHeight()));

		leftTab.setTopRight(toolbarFull);

		leftPanel = new Composite(leftTab, SWT.BORDER); // will hold the top
		// stuff, plus a
		// splitPane
		leftPanel.setLayout(new FormLayout());
		FormData fd4 = new FormData();
		fd4.left = new FormAttachment(0, 5); // attach to left, to top, and
		// to bottom
		fd4.top = new FormAttachment(0, 5);
		fd4.bottom = new FormAttachment(100, -5);
		fd4.right = new FormAttachment(100, -5);
		leftPanel.setLayoutData(fd4);

		Composite topLeft = new Composite(leftPanel, SWT.NONE);
		leftTabItem.setControl(leftPanel);
		FormData fd44 = new FormData();
		fd44.left = new FormAttachment(0, 5); // attach to left, to top, and
		// to right
		fd44.top = new FormAttachment(0, 5);
		fd44.right = new FormAttachment(100, -5);
		topLeft.setLayoutData(fd44);

		topLeft.setLayout(new RowLayout());

		rightTab = new CTabFolder(sash_form, SWT.TOP);
		rightTab.setSimple(false);
		rightTab.setMaximizeVisible(true);
		rightTab.setMinimizeVisible(true);

		rightTab.addCTabFolder2Listener(new CTabFolder2Listener() {
			public void showList(CTabFolderEvent event) {

			}

			public void maximize(CTabFolderEvent event) {
				sash_form.setMaximizedControl(rightTab);
			}

			public void minimize(CTabFolderEvent event) {
				sash_form.setMaximizedControl(leftTab);
			}

			public void close(CTabFolderEvent event) {

			}

			public void restore(CTabFolderEvent event) {

			}
		});

		CTabItem rightTabItem = new CTabItem(rightTab, SWT.NONE);
		rightTabItem.setText("Model Focus");
		rightTab.setSelection(0);

		rightPanel = new Composite(rightTab, SWT.BORDER);
		rightTabItem.setControl(rightPanel);
		rightPanel.setLayout(new FormLayout());
		FormData fd6 = new FormData();
		fd6.top = new FormAttachment(0, 5); // attach to top, to right, and to
		// bottom
		fd6.right = new FormAttachment(100, -5);
		fd6.bottom = new FormAttachment(100, -5);
		rightPanel.setLayoutData(fd6);

		ToolBar toolbarDetail = new ToolBar(rightTab, SWT.HORIZONTAL | SWT.FLAT);

		final ToolItem focus = new ToolItem(toolbarDetail, SWT.PUSH);
		Image focusIcon = new Image(null, "images/e_forward.gif");
		focus.setImage(focusIcon);
		// focus.setText("Focus");
		focus.setToolTipText("Focus On Selected");
		focus.addSelectionListener(new SelectionListener() {
			public void widgetSelected(SelectionEvent event) {
				RefocusButtonPushed();
			}

			public void widgetDefaultSelected(SelectionEvent event) {

			}
		});
		final ToolItem expand = new ToolItem(toolbarDetail, SWT.PUSH);
		Image expandIcon = new Image(null, "images/fjexport.gif");
		expand.setImage(expandIcon);
		// expand.setText("Expand");
		expand.setToolTipText("Expand Selected");
		expand.addSelectionListener(new SelectionListener() {
			public void widgetSelected(SelectionEvent event) {
				detailContents = detailContents
						.getExpandedDetailContents(detailContents.SelectedObject);
				getDetailCanvas().setContents(detailContents);
			}

			public void widgetDefaultSelected(SelectionEvent event) {

			}
		});
		final ToolItem restoreDetail = new ToolItem(toolbarDetail, SWT.PUSH);
		restoreDetail.setImage(restoreIcon);
		// restoreDetail.setText("Restore");
		restoreDetail.setToolTipText("Restore");
		restoreDetail.addSelectionListener(new SelectionListener() {
			public void widgetSelected(SelectionEvent event) {
				sash_form.setMaximizedControl(null);
			}

			public void widgetDefaultSelected(SelectionEvent event) {

			}
		});

		rightTab.setTabHeight(Math.max(toolbarDetail.computeSize(SWT.DEFAULT,
				SWT.DEFAULT).y, rightTab.getTabHeight()));

		rightTab.setTopRight(toolbarDetail);

		Composite topRight = new Composite(rightPanel, SWT.NONE);

		// need to make an explicity layout so that I can set attributes
		RowLayout rowLOright = new RowLayout();
		rowLOright.wrap = true;
		rowLOright.pack = true;
		// rowLOright.justify = true;
		// rowLOright.spacing = 10;
		topRight.setLayout(rowLOright);

		int[] weights = new int[2];
		weights[0] = 75;
		weights[1] = 25;
		sash_form.setWeights(weights);

		// make a composite to hold the "open file stuff"
		//need to initialize witobject
		
		
		fileDialog = new FileDialog(shell, SWT.OPEN);

		
		if (WitDataDir !=null)
			fileDialog.setFilterPath(WitDataDir);
		chooseFocusDialog = new ChooseFocusDialog(shell, this);

		preferencesDialog = new PreferencesDialog(shell, this);
		partOptionDialog = new PartOptionsDialog(shell, this);
		operationOptionDialog = new OperationOptionsDialog(shell, this);
		bomOptionDialog = new BomOptionsDialog(shell, this);
		bopOptionDialog = new BopOptionsDialog(shell, this);
		demandEdgeOptionDialog = new DemandEdgeOptionsDialog(shell, this);
		substituteOptionDialog = new SubstituteOptionsDialog(shell, this);
		globalOptionDialog = new GlobalOptionsDialog(shell, this);

		// add a composite to be used for the opal picture
		// make a sashform to hold just the canvas and the text box stuff
		leftSash = new SashForm(leftPanel, SWT.VERTICAL);
		FormData fdlS = new FormData();
		fdlS.top = new FormAttachment(topLeft, 5);
		fdlS.left = new FormAttachment(0, 5);
		fdlS.right = new FormAttachment(100, -5);
		fdlS.bottom = new FormAttachment(100, -5);
		leftSash.setLayoutData(fdlS);

		FormData fd45 = new FormData();
		fd45.left = new FormAttachment(0, 5); // attach to left, to top, and
		// to right
		fd45.top = new FormAttachment(0, 5);
		fd45.right = new FormAttachment(100, -5);
		// fd45.bottom = new FormAttachment(rightSash,0);
		topRight.setLayoutData(fd45);

		rightSash = new SashForm(rightPanel, SWT.VERTICAL);
		FormData fdrS = new FormData();
		fdrS.top = new FormAttachment(topRight, 5);
		fdrS.left = new FormAttachment(0, 5);
		fdrS.right = new FormAttachment(100, -5);
		fdrS.bottom = new FormAttachment(100, -5);
		rightSash.setLayoutData(fdrS);

		setFigureCanvas(new FigureCanvas(leftSash));
		getFigureCanvas().setContents(contents = getContents());
		getFigureCanvas().getViewport().setContentsTracksHeight(true);
		getFigureCanvas().getViewport().setContentsTracksWidth(true);

		// add a label at the top

		setDetailCanvas(new FigureCanvas(rightSash));
		getDetailCanvas().setContents(contents = getDetailContents());
		getDetailCanvas().getViewport().setContentsTracksHeight(true);
		getDetailCanvas().getViewport().setContentsTracksWidth(true);

		// now we need two composites to hold the texbox+label
		Composite leftText = new Composite(leftSash, SWT.BORDER);
		Composite rightText = new Composite(rightSash, SWT.BORDER);

		leftText.setLayout(new FormLayout());
		rightText.setLayout(new FormLayout());

		leftSash.setWeights(weights);
		rightSash.setWeights(weights);

		org.eclipse.swt.widgets.Label label1 = new org.eclipse.swt.widgets.Label(
				leftText, SWT.LEFT);
		label1.setText("Details");
		FormData fdl1 = new FormData();
		fdl1.top = new FormAttachment(0, 5);
		fdl1.left = new FormAttachment(0, 5);
		fdl1.right = new FormAttachment(100, -5);
		label1.setLayoutData(fdl1);

		org.eclipse.swt.widgets.Label label2 = new org.eclipse.swt.widgets.Label(
				rightText, SWT.LEFT);
		label2.setText("Details");
		FormData fdl2 = new FormData();
		fdl2.top = new FormAttachment(0, 5);
		fdl2.left = new FormAttachment(0, 5);
		fdl2.right = new FormAttachment(100, -5);
		label2.setLayoutData(fdl2);

		TextBox1 = new StyledText(leftText, SWT.LEAD | SWT.MULTI | SWT.WRAP
				| SWT.H_SCROLL | SWT.V_SCROLL | SWT.NONE);
		TextBox1.setText(" ");
		FormData fdt1 = new FormData();
		fdt1.top = new FormAttachment(label1, 5);
		fdt1.left = new FormAttachment(0, 5);
		fdt1.right = new FormAttachment(100, -5);
		fdt1.bottom = new FormAttachment(100, -5);
		TextBox1.setLayoutData(fdt1);

		TextBox2 = new StyledText(rightText, SWT.LEAD | SWT.MULTI | SWT.WRAP
				| SWT.H_SCROLL | SWT.V_SCROLL | SWT.NONE);
		TextBox2.setText("");
		FormData fdt2 = new FormData();
		fdt2.top = new FormAttachment(label2, 5);
		fdt2.left = new FormAttachment(0, 5);
		fdt2.right = new FormAttachment(100, -5);
		fdt2.bottom = new FormAttachment(100, -5);
		TextBox2.setLayoutData(fdt2);

		mainContents.setTextBox(TextBox1);
		detailContents.setTextBox(TextBox2);

		// shell.pack();
		shell.setSize(900, 600);

		if (witFileName!=null)	 {
			if (!GotData()) {
				String message=wo.message;
				displayExit(shell, message, wo.allOK);
			}
			focus.setEnabled(true);
			showglobaldata.setEnabled(true);
		}
			

		shell.open();
		shell.layout(true);
		shell.addDisposeListener(new DisposeListener() {
			public void widgetDisposed(DisposeEvent e) {
				// opalServer.Terminate();
			    
			    try {
			    	if (wo != null)
			    		wo.finalize();
			    }
			    catch (Throwable ex) {
			    	ex.printStackTrace();
			    }

			}
		});

		try {
			while (!shell.isDisposed())
				while (!d.readAndDispatch())
					d.sleep();
		} catch (Throwable t) {
			t.printStackTrace();
			System.exit(0);
		}
	}
    public  void displayExit(Shell s, String message, STATUS allOK) {
		
		TerminalErrorDialog e = new TerminalErrorDialog(shell, this, message, allOK);
		e.open();
	    
		if (allOK==STATUS.DEAD) System.exit(0);

    }
    public  MyMessageDialog displayMessage(Shell s, String message) {
		
		MyMessageDialog e = new MyMessageDialog(shell, message);
		e.open();
	    return e;

    }
	public void OpenFullModel() {
		witFileName = fileDialog.open();
		globalDataDialog.close();

		outputData = false;
		if (witFileName != null) {
			fulltree = true;
			sash_form.setMaximizedControl(null);
			String mess = "Reading data file. "+ longWaitMessage();
			MyMessageDialog e = displayMessage(shell, mess);
			
			
 			if (!GotData()) {
 				e.close();
				String message = wo.message;
				
				displayExit(shell, message, wo.allOK);
				shell.setCursor(null);
				return;
			}
 			
 			focusObject = null;
 			EnableOptions();
 			SetEnabled();
 			e.close();
		}
		
	}
	
	public String WitNeedsToTerminate1() {
		return "WIT issued a severe error message while invoking ";
	}
	
	public String WitNeedsToTerminate2() {
		return "\nAs a result of this severe error, WitViz will need to terminate when this window is closed.  \n The severe error message follows:";
	}
	

	public String longWaitMessage() {
		String mess = "Please wait...\nThis dialog will close when the action is complete";
		return mess;
	}
	public void HeuristicImplosion() throws Exception {
		String mess = "Invoking Heuristic Implosion. "+ longWaitMessage();
		MyMessageDialog e = displayMessage(shell, mess);
		int result = wo.heuristicimplosion();
		e.close();
		if (result==0) {
			SetEnabled();
			outputData = true;
		}
		else {
			String message = WitNeedsToTerminate1()+" Heuristic Implosion " + WitNeedsToTerminate2() + wo.message;
			displayExit(shell, message, wo.allOK);
			shell.setCursor(null);
			return;
		}
	}
	public void OptimizingImplosion() throws Exception{
		String mess = "Invoking Optimizing Implosion. "+longWaitMessage();
		MyMessageDialog e = displayMessage(shell, mess);
		int result = wo.optimizingimplosion();
		e.close();
		if (result==0){
			SetEnabled();
			outputData = true;
		}
		else {
			String message = WitNeedsToTerminate1()+" Optimizing Implosion " + WitNeedsToTerminate2() + wo.message;
			displayExit(shell, message, wo.allOK);
			shell.setCursor(null);
			return;
		}
	}
	public void StochasticImplosion() throws Exception {
		String mess = "Invoking Stochastic Implosion. "+longWaitMessage();
		MyMessageDialog e = displayMessage(shell, mess);
		int result = wo.stochasticimplosion();
		e.close();
		if (result==0) {
			SetEnabled();
			outputData = true;
		}
		else {
			String message = WitNeedsToTerminate1()+" Stochastic Implosion " + WitNeedsToTerminate2() + wo.message;
			displayExit(shell, message, wo.allOK);
			shell.setCursor(null);
			return;
		}
	}
	public void MRP() throws Exception {
		String mess = "Invoking MRP. "+longWaitMessage();
		MyMessageDialog e = displayMessage(shell, mess);
		int result = wo.mrp();
		e.close();
		if (result==0) {
			SetEnabled();
			outputData = true;
		}
		else {
			String message = WitNeedsToTerminate1()+" MRP " + WitNeedsToTerminate2() + wo.message;
			displayExit(shell, message, wo.allOK);
			shell.setCursor(null);
			return;
		}
	}
	public void Postprocess() throws Exception {
	
		String mess = "Invoking Postprocessing. "+longWaitMessage();
		MyMessageDialog e = displayMessage(shell, mess);
		int result = wo.postprocess();
		e.close();
		if (result==0) {
			SetEnabled();
			outputData = true;
		}
		else {
			String message = WitNeedsToTerminate1()+" PostProcessing " + WitNeedsToTerminate2() + wo.message;
			displayExit(shell, message, wo.allOK);
			shell.setCursor(null);
			return;
		}
	}
	
	public void setScenario() {
		scenarioChooser.updateShell();
		scenarioChooser.open();
	}
	public void setObjective() {
		objectiveChooser.updateShell();
		objectiveChooser.open();
	}
	
	public void UpdateScenario(int scenario) {
		wo.setScenario(scenario);
		if (focusObject!=null) {
			if (detailLast&&detailContents.SelectedObject!=null)
				detailContents.refocus((Figure)detailContents.SelectedObject);
			else if (focusObject!=null)
				mainContents.refocus((Figure)focusObject);

		}
		if (globalDataDialog.isOpen==true)
				globalDataDialog.open();
	}
	public void UpdateObjective(String objective) {
		wo.setObjective(objective);
		if (focusObject!=null) {
			if (detailLast&&detailContents.SelectedObject!=null)
				detailContents.refocus((Figure)detailContents.SelectedObject);
			else if (focusObject!=null)
				mainContents.refocus((Figure)focusObject);

		}
		if (globalDataDialog.isOpen==true)
				globalDataDialog.open();
	}
	public void OpenPartialModel() {
		fulltree = false;
		outputData = false;
		getFigureCanvas().setContents(contents = null); // null it out
		sash_form.setMaximizedControl(rightTab); // maximize the sashform to
		globalDataDialog.close();
		
		focusObject = null;
		detailContents.SelectedObject=null;

		// show only the detail
		witFileName = fileDialog.open();
		if (witFileName != null) {
			shell.setText("WitViz" + ": " + witFileName);
			try {
				shell.setCursor(waitCursor);
				String mess = "Reading data file. "+longWaitMessage();
				MyMessageDialog e = displayMessage(shell, mess);
				
				
				wo = new WitObject(witFileName);
				e.close();
				if (wo.allOK!=STATUS.OK) {
					String message=wo.message;
					displayExit(shell,message,wo.allOK);
					shell.setCursor(null);
					return;
				}
				shell.setCursor(null);
				if (wo.getStochMode()==true) {
					setScenario.setEnabled(true);
					scenarioChooser.updateShell();
				}
				else {
					setScenario.setEnabled(false);
					scenarioChooser.close();
				}
				if (wo.getMultObjectiveMode()==true) {
					setObjective.setEnabled(true);
					objectiveChooser.updateShell();
				}
				else {
					setObjective.setEnabled(false);
					objectiveChooser.close();
				}
				
			} catch (Exception ex) {
				ex.printStackTrace();
				
			}
			setWitObject();
			// check for whether we have data for this file yet
			hasOpalData = false;
			 //if (doOpalPicture)
			 //PopulateOpalDataSource(wo);

			focus.setEnabled(true);
			showglobaldata.setEnabled(true);
			focusObject = null;
			EnableOptions();
			SetEnabled();
			

			chooseFocusDialog.open();
			
		}
		
	}

	
	
	public void Print() {
		PrintDialog printd = new PrintDialog(shell);
		// capture shell as it currently appears into an ImageData object
		final org.eclipse.swt.graphics.Rectangle bounds = shell.getBounds();
		final Image srcImage = new Image(d, bounds.width, bounds.height);
		GC srcGC = new GC(d);
		srcGC.copyArea(srcImage, bounds.x, bounds.y);
		srcGC.dispose();
		ImageData srcData = srcImage.getImageData();
		srcImage.dispose();

		PrinterData pd = printd.open();
		final Printer printer;

		if (pd != null) {
			// printer.startJob("foo");
			/*
			 * Do the printing in a background thread so that spooling does not
			 * freeze the UI.
			 */
			printer = new Printer(pd);

			// draw onto printer scaled appropriately
			final org.eclipse.swt.graphics.Point srcDPI = d.getDPI();
			final org.eclipse.swt.graphics.Point destDPI = printer.getDPI();
			org.eclipse.swt.graphics.Rectangle trim = printer.computeTrim(0, 0,
					0, 0);
			final int leftMargin = destDPI.x + trim.x; // one inch from left
			// side of paper
			final int topMargin = destDPI.y / 2 + trim.y; // one-half inch
			// from top edge of
			// paper
			final Image destImage = new Image(printer, srcData);

			Thread printingThread = new Thread("Printing") {
				public void run() {
					print(printer, destImage, bounds, leftMargin, topMargin,
							destDPI, srcDPI);
					printer.dispose();
				}
			};
			printingThread.start();
		}
	}

	public void ChooseFocus() {
		
		chooseFocusDialog.open();
		focusObject = null;
	}

	void RefocusButtonPushed() {

		focusObject = detailContents.SelectedObject;
		if (fulltree) {
			mainContents.setSelected(focusObject);
			mainContents.recenter(focusObject);
		} else {
			focusChanged();

			detailContents.recenter(focusObject);
			detailContents.ColorSelf(focusObject);
		}

	}

	void RefocusButtonPushed(Object f) {

		focusObject = f;

		if (fulltree) {
			mainContents.setSelected(focusObject);
			mainContents.recenter(focusObject);
		} else {
			focusChanged();
			// detailContents.setSelected(focusObject);
			// detailContents.recenter(focusObject);
			// detailContents.ColorSelf(focusObject);
		}

	}

	void print(Printer printer, Image destImage,
			org.eclipse.swt.graphics.Rectangle bounds, int leftMargin,
			int topMargin, org.eclipse.swt.graphics.Point destDPI,
			org.eclipse.swt.graphics.Point srcDPI) {
		if (printer.startJob("SWT Printing Snippet")) {

			GC destGC = new GC(printer);
			if (printer.startPage()) {
				destGC.drawImage(destImage, 0, 0, bounds.width, bounds.height,
						leftMargin, topMargin, bounds.width * destDPI.x
								/ srcDPI.x, bounds.height * destDPI.y
								/ srcDPI.y);
				printer.endPage();
			}
			destGC.dispose();
			printer.endJob();
			destImage.dispose();
		}
		printer.dispose();

	}

	public void setWait() {
		shell.setCursor(waitCursor);
	}

	public void unsetWait() {
		shell.setCursor(null);
	}

	public boolean GotData() {
		shell.setText("WitViz" + ": " + witFileName);
		try {
			shell.setCursor(waitCursor);
			wo = new WitObject(witFileName);
			if (wo.allOK != STATUS.OK) {
				shell.setCursor(null);
				return false;
			}
			shell.setCursor(null);
			focusObject = null;
			detailContents.SelectedObject=null;
			

			
			if (wo.getStochMode()==true) {
				setScenario.setEnabled(true);
				scenarioChooser = new ChooseScenarioDialog(shell, this);

				//scenarioChooser.updateShell();
			}
			else {
				setScenario.setEnabled(false);
				if (scenarioChooser!=null) scenarioChooser.close();
			}
			if (wo.getMultObjectiveMode()==true) {
				setObjective.setEnabled(true);
				objectiveChooser = new ChooseObjectiveDialog(shell, this);

				//objectiveChooser.updateShell();
			}
			else {
				setObjective.setEnabled(false);
				if (objectiveChooser!=null) objectiveChooser.close();
			}
			
				
			
		} catch (Exception ex) {
			ex.printStackTrace();
			
		} catch (Throwable t) {
			System.exit(0);
			
		}
		setWitObject();
		shell.setCursor(null);
		hasOpalData = false;

		// if (doOpalPicture)
		// PopulateOpalDataSource(wo);

		focus.setEnabled(true);
		showglobaldata.setEnabled(true);
		mainContents = (CreateTreeFigure) getContents();
		shell.setCursor(null);
		mainContents.setTextBox(TextBox1);
		focusObject = null;
		EnableOptions();
		SetEnabled();
		detailContents = (CreateTreeFigure) getDetailContents();
		detailContents.setTextBox(TextBox2);
		getFigureCanvas().setContents(mainContents);
		getDetailCanvas().setContents(detailContents);
		
		

		return true;
	}

	private boolean OpalDataExists(String filename) {
		try {
			FileInputStream isl = new FileInputStream(filename + ".Opal.td");
		} catch (Exception e) {
			return false;
		}
		return true;

	}

	private void setWitObject() {
		partOptionDialog.setWitObject(wo);
		bomOptionDialog.setWitObject(wo);
		bopOptionDialog.setWitObject(wo);
		operationOptionDialog.setWitObject(wo);
		demandEdgeOptionDialog.setWitObject(wo);
		substituteOptionDialog.setWitObject(wo);
		globalOptionDialog.setWitObject(wo);
		globalDataDialog.setWitObject(wo);
	}

	// public boolean close() { // copy widgets' input to private members

	// return super.close();
	// }

	// default (English language, United States)

	private void SetEnabled() {
		partOptionDialog.setEnabled();
		operationOptionDialog.setEnabled();
		bomOptionDialog.setEnabled();
		bopOptionDialog.setEnabled();
		substituteOptionDialog.setEnabled();
		demandEdgeOptionDialog.setEnabled();
		globalOptionDialog.setEnabled();

	}

	private void EnableOptions() {
		part.setEnabled(true);
		operation.setEnabled(true);
		BOM.setEnabled(true);
		BOP.setEnabled(true);
		substitute.setEnabled(true);
		demand.setEnabled(true);
		global.setEnabled(true);
		
		//statistics.setEnabled(true);
		postprocess.setEnabled(true);
		heurimplosion.setEnabled(true);
		optimplosion.setEnabled(true);
		stochimplosion.setEnabled(true);
		mrp.setEnabled(true);
		//if (doOpalPicture)
			//statistics.setEnabled(true);
	}

	private void ShowGlobal() {
		
		//int numparts = wo.getNumParts();
		//int numoperations = wo.getNumOperations();
		
		//GlobalOptionsDialog optionsDialog = globalOptionDialog;
		//mainContents.TextBox.setText("Number of Parts: "+ numparts + "\n");
		//.TextBox.append("Number of Operations: "+ numoperations + "\n");
        //optionsDialog.getOptionsString(mainContents.TextBox);

		
		globalDataDialog.open();
	}

	static class TopOrBottomAnchor extends ChopboxAnchor {
		public TopOrBottomAnchor(IFigure owner) {
			super(owner);
		}

		public Point getLocation(Point reference) {
			Point p;
			p = getOwner().getBounds().getCenter();
			getOwner().translateToAbsolute(p);
			if (reference.y < p.y)
				p = getOwner().getBounds().getTop();
			else
				p = getOwner().getBounds().getBottom();
			getOwner().translateToAbsolute(p);
			return p;
		}
	}

	static class TopAnchor extends ChopboxAnchor {
		public TopAnchor(IFigure owner) {
			super(owner);
		}

		public Point getLocation(Point reference) {
			Point p;
			p = getOwner().getBounds().getCenter();
			getOwner().translateToAbsolute(p);

			p = getOwner().getBounds().getTop();

			getOwner().translateToAbsolute(p);
			return p;
		}
	}

	static class BottomAnchor extends ChopboxAnchor {
		public BottomAnchor(IFigure owner) {
			super(owner);
		}

		public Point getLocation(Point reference) {
			Point p;
			p = getOwner().getBounds().getCenter();
			getOwner().translateToAbsolute(p);

			p = getOwner().getBounds().getBottom();

			getOwner().translateToAbsolute(p);
			return p;
		}
	}

	public Vector getSortedPartsList() {
		Vector r = wo.getAllParts();
		Vector returnVec = new Vector();
		Set<String> s = new TreeSet();
		for (int i = 0; i<r.size();i++) {
			s.add((String)r.elementAt(i));
		}
		for (Iterator i=s.iterator();i.hasNext();)	{
			returnVec.add((String)i.next());
		}
		return returnVec;
	}

	public Vector getSortedDemandedPartsList() {
		Vector r = wo.getAllDemandedParts();
		Vector returnVec = new Vector();
		Set<String> s = new TreeSet();
		for (int i = 0; i<r.size();i++) {
			s.add((String)r.elementAt(i));
		}
		for (Iterator i=s.iterator();i.hasNext();)	{
			returnVec.add((String)i.next());
		}
		return returnVec;
	}

	public Vector getSortedCapacitiesList() {
		Vector r = wo.getAllCapacities();
		Vector returnVec = new Vector();
		Set<String> s = new TreeSet();
		for (int i = 0; i<r.size();i++) {
			s.add((String)r.elementAt(i));
		}
		for (Iterator i=s.iterator();i.hasNext();)	{
			returnVec.add((String)i.next());
		}
		return returnVec;
	}

	public String[] getSortedRawMaterialsList() {
		Vector rawmaterials = wo.getRawMaterialsList();
		String[][] returnVec = new String[1][rawmaterials.size()];
		for (int i = 0; i < rawmaterials.size(); i++) {
			ListInfo li = (ListInfo) rawmaterials.elementAt(i);
			returnVec[0][i] = li.name;
		}
		Arrays.sort(returnVec[0]);
		return returnVec[0];
	}

	public Vector getSortedOperationsList() {
		Vector r = wo.getAllOperations();
		Vector returnVec = new Vector();
		Set<String> s = new TreeSet();
		for (int i = 0; i<r.size();i++) {
			s.add((String)r.elementAt(i));
		}
		for (Iterator i=s.iterator();i.hasNext();)	{
			returnVec.add((String)i.next());
		}
		return returnVec;
	}

	protected String[] getGraphMethods() {
		// Method[] methods = GraphTests.class.getMethods();
		Method[] methods = new Method[0];
		String[] methodNames = new String[methods.length];

		int nameIndex = 0;
		for (int i = 0; i < methods.length; i++) {
			if (methods[i].getReturnType().equals(DirectedGraph.class)) {
				methodNames[nameIndex] = methods[i].getName();
				nameIndex++;
			}
		}
		return methodNames;
	}

	public FigureCanvas getFigureCanvas() {
		return figureCanvas;
	}

	public void setFigureCanvas(FigureCanvas figureCanvas) {
		this.figureCanvas = figureCanvas;
	}

}
