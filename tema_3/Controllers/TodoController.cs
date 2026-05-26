using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using TodoApp.Data;
using TodoApp.DTOs;
using TodoApp.Models;

namespace TodoApp.Controllers;

[ApiController]
[Route("api/[controller]")]
public class TodoController : ControllerBase
{
    private readonly TodoDbContext _db;

    public TodoController(TodoDbContext db)
    {
        _db = db;
    }

    [HttpGet]
    public async Task<IActionResult> GetAll()
    {
        var items = await _db.Todos.ToListAsync();
        return Ok(items);
    }

    [HttpGet("{id:int}")]
    public async Task<IActionResult> GetById(int id)
    {
        var item = await _db.Todos.FindAsync(id);
        if (item == null)
            return NotFound();

        return Ok(item);
    }

    [HttpPost]
    public async Task<IActionResult> Create([FromBody] TodoCreateDto dto)
    {
        if (string.IsNullOrWhiteSpace(dto.Title))
            return BadRequest("Titlul este obligatoriu.");

        var item = new TodoItem
        {
            Title = dto.Title,
            Description = dto.Description
        };

        _db.Todos.Add(item);
        await _db.SaveChangesAsync();

        return CreatedAtAction(nameof(GetById), new { id = item.Id }, item);
    }

    [HttpPut("{id:int}")]
    public async Task<IActionResult> Update(int id, [FromBody] TodoUpdateDto dto)
    {
        var item = await _db.Todos.FindAsync(id);
        if (item == null)
            return NotFound();

        item.Title = dto.Title;
        item.Description = dto.Description;
        item.IsCompleted = dto.IsCompleted;

        await _db.SaveChangesAsync();
        return Ok(item);
    }

    [HttpPatch("{id:int}/toggle")]
    public async Task<IActionResult> Toggle(int id)
    {
        var item = await _db.Todos.FindAsync(id);
        if (item == null)
            return NotFound();

        item.IsCompleted = !item.IsCompleted;
        await _db.SaveChangesAsync();

        return Ok(item);
    }

    [HttpDelete("{id:int}")]
    public async Task<IActionResult> Delete(int id)
    {
        var item = await _db.Todos.FindAsync(id);
        if (item == null)
            return NotFound();

        _db.Todos.Remove(item);
        await _db.SaveChangesAsync();

        return NoContent();
    }
}
