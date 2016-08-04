#include <Carbon/Carbon.h>
#include "osx-carbon/carbon_gui.h"
static OSStatus        AppEventHandler(EventHandlerCallRef inCaller, EventRef inEvent, void* inRefcon);
static OSStatus        HandleNew();
static OSStatus        WindowEventHandler(EventHandlerCallRef inCaller, EventRef inEvent, void* inRefcon);

static IBNibRef        sNibRef;

static NavEventUPP navEventHandler;

void carbon_display_dialog(char* output) {
	SInt16 out;
	Str255 pascalOutput;
	Str255 pascalResults;
	CopyCStringToPascal(output, pascalOutput);
	CopyCStringToPascal("Results", pascalResults);
	StandardAlert(kAlertNoteAlert, pascalResults, pascalOutput, NULL, &out);
}

//--------------------------------------------------------------------------------------------
int carbon_create_window(int argc, char* argv[]) {
    OSStatus                    err;
    static const EventTypeSpec    kAppEvents[] =
    {
        { kEventClassCommand, kEventCommandProcess }
    };

    // Create a Nib reference, passing the name of the nib file (without the .nib extension).
    // CreateNibReference only searches into the application bundle.
    err = CreateNibReference(CFSTR("main"), &sNibRef);
    require_noerr(err, CantGetNibRef);
    
    // Once the nib reference is created, set the menu bar. "MainMenu" is the name of the menu bar
    // object. This name is set in InterfaceBuilder when the nib is created.
    err = SetMenuBarFromNib(sNibRef, CFSTR("MenuBar"));
    require_noerr(err, CantSetMenuBar);
    
    // Install our handler for common commands on the application target
    InstallApplicationEventHandler(NewEventHandlerUPP(AppEventHandler),
                                    GetEventTypeCount(kAppEvents), kAppEvents,
                                    0, NULL);
    
    // Create a new window. A full-fledged application would do this from an AppleEvent handler
    // for kAEOpenApplication.
    HandleNew();
    
    // Run the event loop
    RunApplicationEventLoop();

CantSetMenuBar:
CantGetNibRef:
    return err;
}

//--------------------------------------------------------------------------------------------
static OSStatus AppEventHandler(EventHandlerCallRef inCaller, EventRef inEvent, void* inRefcon) {
    OSStatus    result = eventNotHandledErr;
    
    switch (GetEventClass(inEvent)) {
        case kEventClassCommand: {
            HICommandExtended cmd;
            verify_noerr(GetEventParameter(inEvent, kEventParamDirectObject, typeHICommand, NULL, sizeof(cmd), NULL, &cmd));
            
            switch (GetEventKind(inEvent)) {
                case kEventCommandProcess:
                    switch (cmd.commandID) {
                        case kHICommandNew:
                            result = HandleNew();
                            break;
                            
                        // Add your own command-handling cases here
                        
                        default:
                            break;
                    }
                    break;
            }
            break;
        }
            
        default:
            break;
    }
    
    return result;
}

//--------------------------------------------------------------------------------------------
DEFINE_ONE_SHOT_HANDLER_GETTER(WindowEventHandler)

//--------------------------------------------------------------------------------------------
static OSStatus HandleNew() {
    OSStatus                    err;
    WindowRef                    window;
    static const EventTypeSpec    kWindowEvents[] =
    {
        { kEventClassCommand, kEventCommandProcess }
    };
    
    // Create a window. "MainWindow" is the name of the window object. This name is set in 
    // InterfaceBuilder when the nib is created.
    err = CreateWindowFromNib(sNibRef, CFSTR("MainWindow"), &window);
    require_noerr(err, CantCreateWindow);

    // Install a command handler on the window. We don't use this handler yet, but nearly all
    // Carbon apps will need to handle commands, so this saves everyone a little typing.
    InstallWindowEventHandler(window, GetWindowEventHandlerUPP(),
                               GetEventTypeCount(kWindowEvents), kWindowEvents,
                               window, NULL);
    
    // Position new windows in a staggered arrangement on the main screen
    RepositionWindow(window, NULL, kWindowCascadeOnMainScreen);
    
    // The window was created hidden, so show it
    ShowWindow(window);
    
CantCreateWindow:
    return err;
}

void SelectDirectoryCallback(NavEventCallbackMessage callBackSelector, NavCBRecPtr callBackParams, void* callBackUD)
{
	switch (callBackSelector) {
		case kNavCBUserAction: {
			NavReplyRecord reply;
			NavUserAction userAction = 0;
			AEDesc actualDesc;
			FSRef file;
			HFSUniStr255 uniFileName;
			CFStringRef cfFileName;
			char* fileName;
			CFURLRef cfUrl;

			NavDialogGetReply(callBackParams->context, &reply);
			userAction = NavDialogGetUserAction(callBackParams->context);
			
			switch (userAction) {
				case kNavUserActionChoose: {
					AECoerceDesc(&reply.selection, typeFSRef, &actualDesc);
					AEGetDescData(&actualDesc, (void*)(&file), sizeof(FSRef));
					FSGetCatalogInfo(&file, kFSCatInfoNone, NULL, &uniFileName, NULL, NULL);
					
					cfUrl = CFURLCreateFromFSRef(NULL, &file);
					cfFileName = CFURLCopyFileSystemPath(cfUrl, kCFURLPOSIXPathStyle);	
					
					fileName = CFStringGetCStringPtr(cfFileName, kCFStringEncodingMacRoman);
					initialize(fileName);	

					CFRelease(cfUrl);
					CFRelease(cfFileName);
					AEDisposeDesc(&actualDesc);
					break;
				}
			}

			break;
		}
		case kNavCBTerminate: {
			NavDialogDispose(callBackParams->context);
			DisposeNavEventUPP(navEventHandler);
			break;
		}
	}
}

//--------------------------------------------------------------------------------------------
static OSStatus WindowEventHandler(EventHandlerCallRef inCaller, EventRef inEvent, void* inRefcon) {
    WindowRef window = (WindowRef) inRefcon;
    OSStatus    err = eventNotHandledErr;
    NavDialogCreationOptions dialogOptions;
    NavDialogRef dialog;

    ControlID editId = {'text', 0};
    ControlRef ctrl;

    CFStringRef cfText;
    Size actualSize;
    char* text;

    switch (GetEventClass(inEvent)) {
        case kEventClassCommand: {
            HICommandExtended cmd;
            verify_noerr(GetEventParameter(inEvent, kEventParamDirectObject, typeHICommand, NULL, sizeof(cmd), NULL, &cmd));
            
            switch (GetEventKind(inEvent)) {
                case kEventCommandProcess:
                    switch (cmd.commandID) {
                        // Add your own command-handling cases here
                        case 'sele':  // select stop files directory
				NavGetDefaultDialogCreationOptions(&dialogOptions);
				dialogOptions.clientName = CFSTR("langdetect");
				dialogOptions.windowTitle = CFSTR("Select Stop Files Directory");
				dialogOptions.modality = kWindowModalityAppModal;
				
				navEventHandler = NewNavEventUPP(SelectDirectoryCallback);
				
				NavCreateChooseFolderDialog(&dialogOptions, navEventHandler, NULL, NULL, &dialog);
				NavDialogRun(dialog); 				
				break;
			case 'dlan': // detect language
				GetControlByID(window, &editId, &ctrl);
				GetControlData(ctrl, kControlEntireControl, kControlEditTextCFStringTag, sizeof(cfText), &cfText, &actualSize);
				text = CFStringGetCStringPtr(cfText, kCFStringEncodingMacRoman);
				detect_language(text);
				break;
                        default:
                            break;
                    }
                    break;
            }
            break;
        }
            
        default:
            break;
    }
    
    return err;
}
