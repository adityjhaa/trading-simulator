const burger = document.querySelector(".navbar-click")
const links = document.querySelector(".links")
const closed = document.querySelector(".close")


burger.addEventListener("click",()=>{
    burger.classList.toggle("active")
    links.classList.toggle("active")

})

document.querySelectorAll(".nav-link").forEach(n => n.addEventListener("click", ()=>{
    burger.classList.remove("active")
    links.classList.remove("active")
}))

closed.addEventListener("click",()=>{
    closed.classList.toggle("active")
})