using System;
using System.Collections.Generic;
using Microsoft.Extensions.Logging;
using Microsoft.Extensions.Configuration;

namespace LongLineSample
{
    public class ComplexSystem
    {
        private readonly ILogger<ComplexSystem> _logger;
        private readonly IConfiguration _configuration;
        private readonly IServiceProvider _serviceProvider;
        
        // This is a very long constructor signature that would normally overflow the page width in PDF output without proper line wrapping configuration
        public ComplexSystem(ILogger<ComplexSystem> logger, IConfiguration configuration, IServiceProvider serviceProvider, IDataRepository dataRepository, ICacheService cacheService, INotificationService notificationService)
        {
            _logger = logger ?? throw new ArgumentNullException(nameof(logger), "Logger cannot be null and must be properly configured with appropriate log levels");
            _configuration = configuration ?? throw new ArgumentNullException(nameof(configuration), "Configuration is required for system initialization");
            _serviceProvider = serviceProvider ?? throw new ArgumentNullException(nameof(serviceProvider), "Service provider is essential for dependency injection");
        }
        
        // Another example of a long line with a complex LINQ query
        public IEnumerable<ProcessedDataItem> ProcessDataWithComplexFiltering(IEnumerable<RawDataItem> rawData)
        {
            return rawData.Where(item => item.IsValid && item.Timestamp > DateTime.Now.AddDays(-30) && item.Category == "Important" && item.Status != DataStatus.Deleted)
                          .Select(item => new ProcessedDataItem { Id = item.Id, Name = item.Name, ProcessedAt = DateTime.Now, AdditionalInfo = $"Processed from {item.Source} at {DateTime.Now:yyyy-MM-dd HH:mm:ss}" })
                          .OrderBy(item => item.ProcessedAt)
                          .ThenBy(item => item.Name);
        }
    }
}