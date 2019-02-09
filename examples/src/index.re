[%bs.raw {|require('./index.css')|}];

[@bs.module "./serviceWorker"]
external registerServiceWorker: unit => unit = "register";
[@bs.module "./serviceWorker"]
external unregisterServiceWorker: unit => unit = "unregister";

ReactDOMRe.renderToElementWithId(
  React.createElement(App.make, App.makeProps()),
  "root",
);