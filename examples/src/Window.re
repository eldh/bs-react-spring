type listenerId;

[@bs.val] [@bs.scope "window"]
external addWindowEventListener: (string, 'a => unit) => listenerId =
  "addEventListener";

[@bs.val] [@bs.scope "window"]
external removeWindowEventListener: (string, listenerId) => unit =
  "removeEventListener";

[@bs.val] [@bs.scope "window"] external windowWidth: int = "innerWidth";
[@bs.val] [@bs.scope "window"] external windowHeight: int = "innerHeight";

let useWindowSize = () => {
  let ((width, height), setSize) =
    React.useState(() => (windowWidth, windowHeight));
  React.useEffect0(
    () => {
      let handleResize = _ => setSize(_ => (windowWidth, windowHeight));
      let listener = addWindowEventListener("resize", handleResize);
      Some(() => removeWindowEventListener("resize", listener));
    },
  );
  (width, height);
};