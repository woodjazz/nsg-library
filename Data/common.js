// Javascript module pattern:
//   see http://en.wikipedia.org/wiki/Unobtrusive_JavaScript#Namespaces
// In essence, we define an anonymous function which is immediately called and
// returns a new object. The new object contains only the exported definitions;
// all other definitions in the anonymous function are inaccessible to external
// code.
var common = (function() {

/**
   * Called when the DOM content has loaded; i.e. the page's document is fully
   * parsed. At this point, we can safely query any elements in the document via
   * document.querySelector, document.getElementById, etc.
**/   
  function domContentLoaded(id, src, attrs) 
  {
    // If the page loads before the Native Client module loads, then set the
    // status message indicating that the module is still loading.  Otherwise,
    // do not change the status message.
    updateStatus('Page loaded.');
	
  	if (common.naclModule == null) 
  	{
      navigator.webkitPersistentStorage.requestQuota(1024 * 1024,
          function(bytes) {
            common.updateStatus(
                'Allocated ' + bytes + ' bytes of persistant storage.');

            // We use a non-zero sized embed to give Chrome space to place the bad
            // plug-in graphic, if there is a problem.
            attachDefaultListeners();
            common.naclModule = createNaClModule(id, src, attrs);
          },
          function(e) { alert('Failed to allocate space') });
    } 
  	else 
    {
      // It's possible that the Native Client module onload event fired
      // before the page's onload event.  In this case, the status message
      // will reflect 'SUCCESS', but won't be displayed.  This call will
      // display the current message.
      updateStatus('Waiting.');
    }
  }
 
  /**
   * Inject a script into the DOM, and call a callback when it is loaded.
   *
   * @param {string} url The url of the script to load.
   * @param {Function} onload The callback to call when the script is loaded.
   * @param {Function} onerror The callback to call if the script fails to load.
   */
  function injectScript(url, onload, onerror) {
    var scriptEl = document.createElement('script');
    scriptEl.type = 'text/javascript';
    scriptEl.src = url;
    scriptEl.onload = onload;
    if (onerror) {
      scriptEl.addEventListener('error', onerror, false);
    }
    document.head.appendChild(scriptEl);
  }

    /**
   * Create the Native Client <embed> element as a child of the DOM element
   * named "listener".
   */
  function createNaClModule(id, src, attrs) {
    var moduleEl = document.createElement('embed');
    moduleEl.setAttribute('id', id);
    moduleEl.setAttribute('width', '100%');
    moduleEl.setAttribute('height', '100%');
    moduleEl.setAttribute('src', src);

    // Add any optional arguments
    if (attrs) {
      for (var key in attrs) {
        moduleEl.setAttribute(key, attrs[key]);
      }
    }

    moduleEl.setAttribute('type', "application/x-pnacl");

    // The <EMBED> element is wrapped inside a <DIV>, which has both a 'load'
    // and a 'message' event listener attached.  This wrapping method is used
    // instead of attaching the event listeners directly to the <EMBED> element
    // to ensure that the listeners are active before the NaCl module 'load'
    // event fires.
    var listenerDiv = document.getElementById('listener');
    listenerDiv.appendChild(moduleEl);
	
	return moduleEl;
  }
  
  /**
   * Add the default "load" and "message" event listeners to the element with
   * id "listener".
   *
   * The "load" event is sent when the module is successfully loaded. The
   * "message" event is sent when the naclModule posts a message using
   * PPB_Messaging.PostMessage() (in C) or pp::Instance().PostMessage() (in
   * C++).
   */
  function attachDefaultListeners() {
    var listenerDiv = document.getElementById('listener');
    listenerDiv.addEventListener('load', moduleDidLoad, true);
    listenerDiv.addEventListener('message', handleMessage, true);
    listenerDiv.addEventListener('crash', handleCrash, true);
    if (typeof window.attachListeners !== 'undefined') {
      window.attachListeners();
    }
  }
  
  /**
   * Called when the NaCl module is loaded.
   *
   * This event listener is registered in attachDefaultListeners above.
   */
  function moduleDidLoad() {

	updateStatus('RUNNING');

    if (typeof window.moduleDidLoad !== 'undefined') {
      window.moduleDidLoad();
    }
  }

  /**
   */
  var defaultMessageTypes = {
    'alert': alert,
    'log': logMessage
  };

  /**
   * Called when the NaCl module sends a message to JavaScript (via
   * PPB_Messaging.PostMessage())
   *
   * This event listener is registered in createNaClModule above.
   *
   * @param {Event} message_event A message event. message_event.data contains
   *     the data sent from the NaCl module.
   */
  function handleMessage(message_event) {
    if (typeof message_event.data === 'string') {
      for (var type in defaultMessageTypes) {
        if (defaultMessageTypes.hasOwnProperty(type)) {
          if (startsWith(message_event.data, type + ':')) {
            func = defaultMessageTypes[type];
            func(message_event.data.slice(type.length + 1));
            return;
          }
        }
      }
    }

    if (typeof window.handleMessage != 'undefined') {
      window.handleMessage(message_event);
      return;
    }

    logMessage('Unhandled message: ' + message_event.data);
  }
  
 
   /**
   * Called when the Browser can not communicate with the Module
   *
   * This event listener is registered in attachDefaultListeners above.
   */
  function handleCrash(event) {
    if (common.naclModule.exitStatus == -1) {
      updateStatus('CRASHED');
    } else {
      updateStatus('EXITED [' + common.naclModule.exitStatus + ']');
    }
    if (typeof window.handleCrash != 'undefined') {
      window.handleCrash(common.naclModule.lastError);
    }
  }


  function hideModule() {
    // Setting common.naclModule.style.display = "None" doesn't work; the
    // module will no longer be able to receive postMessages.
    common.naclModule.style.height = '0';
  }
  
  /**
   * Remove the NaCl module from the page.
   */
  function removeModule() {
    common.naclModule.parentNode.removeChild(common.naclModule);
    common.naclModule = null;
  }
  
  /**
   * Return true when |s| starts with the string |prefix|.
   *
   * @param {string} s The string to search.
   * @param {string} prefix The prefix to search for in |s|.
   */
  function startsWith(s, prefix) {
    // indexOf would search the entire string, lastIndexOf(p, 0) only checks at
    // the first index. See: http://stackoverflow.com/a/4579228
    return s.lastIndexOf(prefix, 0) == 0;
  }

  /** Maximum length of logMessageArray. */
  var kMaxLogMessageLength = 20;

  /** An array of messages to display in the element with id "log". */
  var logMessageArray = [];

  /**
   * Add a message to an element with id "log".
   *
   * This function is used by the default "log:" message handler.
   *
   * @param {string} message The message to log.
   */
  function logMessage(message) {
    logMessageArray.push(message);
    if (logMessageArray.length > kMaxLogMessageLength)
      logMessageArray.shift();

	  var logE = document.getElementById('log');
	  if(logE != null)
		logE.textContent = logMessageArray.join('\n');
    console.log(message);
  }
  
  /** Saved text to display in the element with id 'statusField'. */
  var statusText = 'NO-STATUSES';

  /**
   * Set the global status message. If the element with id 'statusField'
   * exists, then set its HTML to the status message as well.
   *
   * @param {string} opt_message The message to set. If null or undefined, then
   *     set element 'statusField' to the message from the last call to
   *     updateStatus.
   */
  function updateStatus(opt_message) {
    if (opt_message) {
      statusText = opt_message;
    }
    var statusField = document.getElementById('statusField');
    if (statusField) {
      statusField.innerHTML = statusText;
    }
  }

// The symbols to export.
  return {
  
    /** A reference to the NaCl module, once it is loaded. */
    naclModule: null,

    attachDefaultListeners: attachDefaultListeners,
    domContentLoaded: domContentLoaded,
    createNaClModule: createNaClModule,
    hideModule: hideModule,
    removeModule: removeModule,
    logMessage: logMessage,
    updateStatus: updateStatus
  };

}());

// Listen for the DOM content to be loaded. This event is fired when parsing of
// the page's document has finished.
document.addEventListener('DOMContentLoaded', function() {
	common.domContentLoaded("nsg_pnacl", "nsg-library.nmf");
});
