(function () {
  var params = new URLSearchParams(window.location.search);

  if (params.has('native')) {
    var nativeScript = document.createElement('script');
    nativeScript.src = 'https://oi.esphome.io/v2/www.js';
    document.body.appendChild(nativeScript);
    return;
  }

  window.location.replace('/coffee');
})();
