// Shared helpers for the dashboard pages: menu, driver selection, telemetry and gauges.
// Exposed as window.DashboardCommon; everything else stays private to the closure.
window.DashboardCommon = (() => {
  // sessionStorage key holding the selected driver ("Driver 1".."Driver 6")
  const driverKey = "fs-selected-driver";
  // True only for "Driver 1" to "Driver 6"; null/undefined are treated as invalid
  const validDriver = (name) => /^Driver [1-6]$/.test(name ?? "");

  // Dropdown menu: toggle button, click-outside and Escape to close,
  // and a "dashboard-menu-action" event for items with data-menu-action.
  function initMenu(rootSelector = ".dashboard-menu") {
    const root = document.querySelector(rootSelector);
    if (!root) return;
    const toggle = root.querySelector(".menu-toggle");
    const panel = root.querySelector(".menu-panel");
    const close = () => {
      toggle.setAttribute("aria-expanded", "false");
      panel.hidden = true;
    };

    toggle.addEventListener("click", () => {
      const expanded = toggle.getAttribute("aria-expanded") === "true";
      toggle.setAttribute("aria-expanded", String(!expanded));
      panel.hidden = expanded;
    });
    document.addEventListener("click", (event) => {
      // Close when clicking outside the menu
      if (!root.contains(event.target)) close();
      // Re-dispatch clicks on menu items as a custom event with the action name
      const item = event.target.closest("[data-menu-action]");
      if (item && panel.contains(item)) document.dispatchEvent(new CustomEvent("dashboard-menu-action", {
        detail: { action: item.dataset.menuAction },
      }));
    });
    document.addEventListener("keydown", (event) => {
      if (event.key === "Escape") {
        close();
        toggle.focus();
      }
    });
  }

  // Show the Personal Bests link only when a valid driver is stored.
  // Hides it if sessionStorage is unavailable. `context` is used in the error log.
  function syncPersonalBestsLink(context) {
    try {
      const link = document.querySelector("#personal-bests-link");
      if (link) link.hidden = !validDriver(sessionStorage.getItem(driverKey));
    } catch (error) {
      const link = document.querySelector("#personal-bests-link");
      if (link) link.hidden = true;
      console.error(`Could not check the selected driver for ${context}.`, error);
    }
  }

  // Write the selected driver's name into `selector` and toggle the Personal Bests link.
  // fallback: text when no valid driver is stored
  // clearDefault: clear the stored driver first (resets to the default view)
  // errorFallback: text when sessionStorage cannot be read
  function showSelectedDriver(
    selector,
    context,
    fallback = "Default Driver",
    clearDefault = false,
    errorFallback = "Driver Name",
  ) {
    const element = document.querySelector(selector);
    try {
      if (clearDefault) sessionStorage.removeItem(driverKey);
      const selected = sessionStorage.getItem(driverKey);
      if (validDriver(selected)) {
        element.textContent = selected;
        const link = document.querySelector("#personal-bests-link");
        if (link) link.hidden = false;
      } else {
        element.textContent = fallback;
        const link = document.querySelector("#personal-bests-link");
        if (link) link.hidden = true;
      }
    } catch (error) {
      element.textContent = errorFallback;
      const link = document.querySelector("#personal-bests-link");
      if (link) link.hidden = true;
      console.error(`Could not read the selected driver for ${context}.`, error);
    }
  }

  // Driver picker page: clicking a .driver-card saves "Driver N" to the session,
  // emits "driver-selected" and navigates to default-driver.html.
  function initDriverSelection() {
    syncPersonalBestsLink("Personal Bests access");
    document.querySelectorAll(".driver-card").forEach((card) => {
      card.addEventListener("click", () => {
        // Mark only the clicked card as pressed
        document.querySelectorAll(".driver-card").forEach((item) =>
          item.setAttribute("aria-pressed", String(item === card)));
        const name = `Driver ${card.dataset.driver}`;
        try {
          sessionStorage.setItem(driverKey, name);
        } catch (error) {
          console.error("Could not save the selected driver for this session.", error);
        }
        document.dispatchEvent(new CustomEvent("driver-selected", {
          detail: { driver: Number(card.dataset.driver), name },
        }));
        window.location.assign("default-driver.html");
      });
    });
    initMenu(".menu");
  }

  // Page guard: returns true if a valid driver is selected, otherwise logs the
  // reason, redirects to `redirect` (replace, so Back does not loop) and returns false.
  function requireSelectedDriver(pageName, redirect) {
    try {
      const selected = sessionStorage.getItem(driverKey);
      if (selected === null) {
        const error = new Error(`${pageName} requires a selected driver, but no driver was found.`);
        error.name = "SelectedDriverAbsentError";
        throw error;
      }
      if (!validDriver(selected)) {
        const error = new Error(`${pageName} cannot open with an invalid driver selection: "${selected}".`);
        error.name = "SelectedDriverInvalidError";
        throw error;
      }
      return true;
    } catch (error) {
      // Keep our own errors; wrap anything else (e.g. storage access errors)
      const selectionError = error instanceof Error &&
        ["SelectedDriverAbsentError", "SelectedDriverInvalidError"].includes(error.name)
        ? error
        : Object.assign(new Error(`The selected driver is unavailable for ${pageName}.`, { cause: error }),
          { name: "SelectedDriverUnavailableError" });
      console.error(selectionError);
      window.location.replace(redirect);
      return false;
    }
  }

  // Validate readings against `fields` and return the DOM updates to apply.
  // Touches no DOM, so one bad reading cannot leave the display half-updated.
  // fields: { key: [selector, digits = 0, pad = 0, minimum = -Infinity, maximum = Infinity] }
  // name: label used in error messages; requireValues: reject an empty readings object
  function prepareTelemetry(readings, fields, name, requireValues = false) {
    if (!readings || typeof readings !== "object" || Array.isArray(readings)) {
      throw new TypeError(`${name} telemetry readings must be an object.`);
    }
    const entries = Object.entries(readings);
    if (requireValues && !entries.length) {
      throw new TypeError(`At least one ${name.toLowerCase()} telemetry reading must be provided.`);
    }
    const updates = entries.map(([key, value]) => {
      // Only configured fields are accepted
      if (!Object.hasOwn(fields, key)) throw new RangeError(`Unknown ${name.toLowerCase()} telemetry field: ${key}`);
      const field = fields[key];
      if (typeof value !== "number" || !Number.isFinite(value)) {
        throw new TypeError(`Telemetry value "${key}" must be a finite number.`);
      }
      const [selector, digits = 0, pad = 0, minimum = -Infinity, maximum = Infinity] = field;
      if (value < minimum) {
        if (minimum === 0) throw new RangeError(`Telemetry value "${key}" must not be negative.`);
        throw new RangeError(`Telemetry value "${key}" is outside its supported range.`);
      }
      if (value > maximum) {
        throw new RangeError(`Telemetry value "${key}" is outside its supported range.`);
      }
      return { key, value, selector, digits, pad };
    });
    return updates;
  }

  // Write prepared updates to the page: fixed decimals if `digits` is set,
  // otherwise a rounded integer zero-padded to `pad` characters.
  function applyTelemetry(updates) {
    updates.forEach(({ value, selector, digits, pad }) => {
      const text = digits ? value.toFixed(digits) : String(Math.round(value)).padStart(pad, "0");
      document.querySelector(selector).textContent = text;
    });
  }

  // Validate and apply in one call
  function updateTelemetry(readings, fields, name, requireValues = false) {
    applyTelemetry(prepareTelemetry(readings, fields, name, requireValues));
  }

  // Voltage gauge: 32 ticks across a 270 degree arc (-135 to +135) around (92, 92).
  // Zones: 8 plain, 10 green, 6 yellow, 4 orange, 4 red. Ticks animate in with a staggered delay.
  function initVoltageTicks(selector = ".voltage-ticks") {
    const svg = document.querySelector(selector);
    if (!svg) return;
    const colors = [...Array(8).fill(""), ...Array(10).fill("zone-green"),
      ...Array(6).fill("zone-yellow"), ...Array(4).fill("zone-orange"), ...Array(4).fill("zone-red")];
    colors.forEach((color, index) => {
      const tick = document.createElementNS("http://www.w3.org/2000/svg", "rect");
      tick.setAttribute("class", `voltage-tick ${color}`.trim());
      tick.setAttribute("x", "86.5");
      tick.setAttribute("y", "2");
      tick.setAttribute("width", "11");
      tick.setAttribute("height", "22");
      tick.setAttribute("transform", `rotate(${-135 + (270 / 31) * index} 92 92)`);
      tick.style.animationDelay = `${index * 18}ms`;
      svg.append(tick);
    });
  }

  // Pack regen dial: 32 coloured ticks around a full circle (360 / 32 = 11.25 degrees each),
  // centred on (109, 109.5). Colours are listed per tick in order.
  function initPackRegenDial(selector = ".pack-regen-ticks") {
    const svg = document.querySelector(selector);
    if (!svg) return;
    const colors = [
      "#26FF33", "#26FF33", "#26FF33", "#A6FF0D", "#A6FF0D", "#A6FF0D",
      "#FFCC0D", "#FFCC0D", "#FFCC0D", "#FFCC0D", "#FFCC0D", "#F8712E",
      "#F8712E", "#D23715", "#F61111", "#F61111", "#576375", "#576375",
      "#576375", "#576375", "#576375", "#576375", "#576375", "#576375",
      "#576375", "#576375", "#26FF33", "#26FF33", "#26FF33", "#26FF33",
      "#26FF33", "#26FF33",
    ];
    colors.forEach((color, index) => {
      const tick = document.createElementNS("http://www.w3.org/2000/svg", "rect");
      tick.setAttribute("x", "102.86");
      tick.setAttribute("y", "2");
      tick.setAttribute("width", "12.29");
      tick.setAttribute("height", "20.16");
      tick.setAttribute("rx", "1");
      tick.setAttribute("fill", color);
      tick.setAttribute("transform", `rotate(${index * 11.25} 109 109.5)`);
      // Glow in the tick's own colour
      tick.style.filter = `drop-shadow(0 0 1px ${color})`;
      svg.append(tick);
    });
  }

  return {
    query: (selector) => document.querySelector(selector),
    initMenu,
    syncPersonalBestsLink,
    showSelectedDriver,
    initDriverSelection,
    requireSelectedDriver,
    prepareTelemetry,
    applyTelemetry,
    updateTelemetry,
    initVoltageTicks,
    initPackRegenDial,
  };
})();