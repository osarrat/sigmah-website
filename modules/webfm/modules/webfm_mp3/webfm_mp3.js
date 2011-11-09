// $Id: webfm_mp3.js,v 1.1 2009/02/08 06:25:40 robmilne Exp $
/**
 * @file webfm_mp3.js
 *
 * Handles AJAX submission and response in WebFM Browser UI.
 */
function webfm_mp3() {}

Webfm.menu_msg["playmp3"] = "Play mp3 file";
webfm_mp3.audioCont = null;

if (Drupal.jsEnabled) {
  $(window).load(webfm_mp3_GetMenusAjax);
}

webfm_mp3.ajaxUrl = function() {
  var path = getBaseUrl() + "/?q=webfm_mp3_js";
  return path;
}

function webfm_mp3_GetMenusAjax() {
  var url = webfm_mp3.ajaxUrl();
  Webfm.progressObj.show(Webfm.js_msg["work"],  "blue");
  var postObj = {action:encodeURIComponent("checkmp3")};
  Webfm.HTTPPost(url, webfm_mp3.GetMenusCallback, '', postObj);
}

webfm_mp3.GetMenusCallback = function(string, xmlhttp, obj) {
  Webfm.progressObj.hide();
  Webfm.alrtObj.msg();
  if (xmlhttp.status == 200) {
    var result = Drupal.parseJson(string);
    Webfm.dbgObj.dbg("GetMenusCallback:", Webfm.dump(result));
    if (result.status) {
      webfm_mp3.audioObj = new webfm_mp3.audio(result.data.id, result.data.title, result.data.width, result.data.height);

      Webfm.menuHT.put('file', new Webfm.menuElement(Webfm.menu_msg["playmp3"], webfm_mp3.menuPlayMp3, webfm_mp3.menuMp3));
    } else {
      Webfm.alrtObj.msg(result.data);
    }
  } else {
    Webfm.alrtObj.msg(Webfm.js_msg["ajax-err"]);
  }
}

webfm_mp3.menuPlayMp3 = function(obj) {
  var url = webfm_mp3.ajaxUrl();
  var path = obj.element.title;
  Webfm.progressObj.show(Webfm.js_msg["work"],  "blue");
  var postObj = {action:encodeURIComponent("playmp3"), filepath:encodeURIComponent(path)};
  Webfm.HTTPPost(url, webfm_mp3.menuPlayMp3Callback, obj, postObj);
}

webfm_mp3.menuPlayMp3Callback = function(string, xmlhttp, obj) {
  Webfm.progressObj.hide();
  Webfm.alrtObj.msg();
  if (xmlhttp.status == 200) {
    var result = Drupal.parseJson(string);
//    Webfm.dbgObj.dbg("mp3 callback:", Webfm.dump(result));
    if (result.status) {
      webfm_mp3.audioObj.show(result.data.path, result.data.filename);
    } else {
      Webfm.alrtObj.msg(result.data);
    }
  } else {
    Webfm.alrtObj.msg(Webfm.js_msg["ajax-err"]);
  }
}

webfm_mp3.menuMp3 = function(obj) {
  // mp3 file must be in database
  if(obj.element.id.substring(0,3) == 'fid' && obj.ext.toLowerCase() == 'mp3')
    return true;
  return false;
}

/**
 * Webfm.audio constructor
 * 1st param is popup container obj
 * 2nd param is default startup width
 * 2nd param is default startup height
 */
webfm_mp3.audio = function(id, title, width, height) {
  //create audio popup
  this.container = new Webfm.popup(id + "-container");
  this.id = id;
  this.title = title;
  this.width = width;
  this.height = height;
  this.pane = null;
}

webfm_mp3.audio.prototype.show = function(path, filename) {
  if(this.container.isEmpty)
    this.pane = new Webfm.pane(this.container, this.title, this.id, null, this.width, this.height);
  this.pane.headerMsg(filename);
  AudioPlayer.embed(this.id + '-content', {soundFile: getBaseUrl() + "/?q=" + path});
  this.pane.show();
}
