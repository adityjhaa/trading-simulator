const links = document.querySelector(".links")
const burger = document.querySelector(".navbar-click")

burger.addEventListener("click", () => {
    burger.classList.toggle("active")
    links.classList.toggle("active")

})

document.querySelectorAll(".nav-link").forEach(n => n.addEventListener("click", () => {
    burger.classList.remove("active")
    links.classList.remove("active")
}))
