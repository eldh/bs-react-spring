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
    ReactHooks.useState((windowWidth, windowHeight));
  ReactHooks.useEffect(
    () => {
      let handleResize = _ => setSize(. (windowWidth, windowHeight));
      let listener = addWindowEventListener("resize", handleResize);
      Some((.) => removeWindowEventListener("resize", listener));
    },
    [||],
  );
  (width, height);
};