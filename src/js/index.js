const mbid = document.getElementById("mbid");
const mtid = document.getElementById("mtid");

function sM(c, t) {
  mtid.classList = c;
  mtid.innerHTML = t;
  s(mbid);
}

function hM() {
    h(mbid);
}

function s(e) {
  e.style.display = "block";
}

function h(e) {
  e.style.display = "none";
}

document.addEventListener("DOMContentLoaded", () => {
  // FORM
  document.querySelectorAll(".ajax-form").forEach((form) => {
    form.addEventListener("submit", function (e) {
      e.preventDefault();
      const formData = new FormData(this);
      const urlsearchparams = new URLSearchParams(formData);
      fetch(this.action, {
        method: this.method || "POST",
        headers: {
          "Content-Type": "application/x-www-form-urlencoded",
        },
        body: urlsearchparams.toString(),
      })
        .then((response) => {
          if (!response.ok)
            throw new Error(response.status + " " + response.statusText);
          else return response.text();
        })
        .then((data) => {
          sM("h4 bd bd-ss my-3", data);
        })
        .catch((error) => {
          sM("h4 bd bd-dg my-3", error);
        });
    });
  });

  // HEADER
  const headerContainer = document.getElementById("ajax-header");
  if (headerContainer) {
    fetch("header.min.html")
      .then((response) => {
        if (!response.ok)
          throw new Error(
            response.status + " " + response.statusText + ". Cannot load Header"
          );
        return response.text();
      })
      .then((data) => {
        headerContainer.innerHTML = data;
        const currentPath = window.location.pathname.split("/").pop();
        document.querySelectorAll(".nv a").forEach((link) => {
          let linkPath = link.getAttribute("href").split("/").pop();
          if (linkPath === currentPath) {
            link.classList.add("active");
            return;
          }
        });
      })
      .catch((error) => console.error(error));
  } else
    console.error("No element with class 'ajax-header' found in the document.");
});
