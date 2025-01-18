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
          else response.text();
        })
        .then((data) => {
          alert(data);
        })
        .catch((error) => {
          alert(error);
        });
    });
  });

  // HEADER
  const headerContainer = document.getElementById("ajax-header");
  if (headerContainer) {
    fetch("header.min.html")
      .then((response) => {
        if (!response.ok) {
          throw new Error("Fetch: Cannot load header");
        }
        return response.text();
      })
      .then((data) => {
        headerContainer.innerHTML = data;
        const currentPath = window.location.pathname.split("/").pop();
        document.querySelectorAll(".navegation a").forEach((link) => {
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
