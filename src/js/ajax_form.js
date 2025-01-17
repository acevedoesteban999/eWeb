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
        response.text().then((data) => {
          if (response.ok) alert(data);
          else
            alert(
              "Error at send form. Status" +
                response.status +
                ". Message:" +
                data
            );
        });
      })
      .catch((error) => {
        alert(error);
      });
  });
});
